#include "WaterSurface.h"
#include "Entity.h"
#include "Camera.h"
#include "GameEngine.h"
#include "HeightMapResource.h"
#include "TextureResource.h"
#include "RenderPass.h"
#include "Timer.h"
#include "Model.h"
#include <iostream>
namespace Engine {

	void WaterRenderOperation::Execute() {
	
		auto component = static_cast<WaterSurface*>(this->GetComponent());
		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glEnable(GL_PRIMITIVE_RESTART));
			DEBUG_OGL(glPrimitiveRestartIndex(mesh->restartIndex));
		}

		DEBUG_OGL(glUniform1i(pass->GetRenderTypeUniform(), 2));

		auto currentTexture = pass->GetNumShadowMaps();

		DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + currentTexture));
		DEBUG_OGL(glUniform1i(pass->GetWaterNormalMapUniform(), currentTexture));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->GetNormalMap()->GetTextureId()));

		currentTexture++;

		DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + currentTexture));
		DEBUG_OGL(glUniform1i(pass->GetWaterUVDVMapUniform(), currentTexture));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->GetDuDv()->GetTextureId()));

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
		DEBUG_OGL(glBindVertexArray(0));

		float texOffset = component->getTexAlpha();
		float waveOffset = component->getWaveAlpha();

		DEBUG_OGL(glUniform1f(pass->GetWaveOffsetUniform(), waveOffset));
		DEBUG_OGL(glUniform1f(pass->GetTexOffsetUniform(), texOffset));

		cout << "WaveOffset: " << waveOffset << endl;

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glDisable(GL_PRIMITIVE_RESTART));
		}	
	}

	void WaterSurface::Wire() {
	
	}

	void WaterSurface::Init() {
		if (this->resource != nullptr)
		{
			this->resource->Init();
			this->normalmap->Init();
			this->dudv->Init();
		}

		for (auto &mesh : this->resource->GetMeshes())
		{
			GetEngine()->GetRenderPass()->AddOperation(new WaterRenderOperation(mesh, this));
		}
	}

	WaterSurface::WaterSurface() {
		auto mapSize = vec3(1024, 40, 1024);
		this->resource = new HeightMapResource("textures/water.png", mapSize, 10, 10);

		this->normalmap = new TextureResource("textures/waternormal.png");
		this->dudv = new TextureResource("textures/dudvmap.jpg");
		
		this->texOffset = new Timer(true,20.0);
		this->waveOffset = new Timer(true,18.0);
	}

	WaterSurface::WaterSurface(int size) {
	
	}

	WaterSurface::~WaterSurface() {
	
	}

	HeightMapResource * WaterSurface::GetResource() const
	{
		return this->resource;
	}

	TextureResource * WaterSurface::GetNormalMap() const
	{
		return this->normalmap;
	}

	TextureResource * WaterSurface::GetDuDv() const
	{
		return this->dudv;
	}

	float WaterSurface::getTexAlpha() {
		return this->texOffset->GetAlpha();
	}

	float WaterSurface::getWaveAlpha() {
		return this->waveOffset->GetAlpha();
	}

}