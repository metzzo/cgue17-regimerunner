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
		if (component->GetEngine()->GetMainCamera()->IsClippingEnabled())
		{
			return;
		}

		auto pass = static_cast<RenderPass*>(this->GetPass());

		pass->SetDrawingTransform(component->GetTransformation());

		if (mesh->restartIndex != -1)
		{
			DEBUG_OGL(glEnable(GL_PRIMITIVE_RESTART));
			DEBUG_OGL(glPrimitiveRestartIndex(mesh->restartIndex));
		}

		if (component->GetEngine()->IsWaterEnabled()) {
			DEBUG_OGL(glUniform1i(pass->GetRenderTypeUniform(), RT_WATER));
		}
		else {
			DEBUG_OGL(glUniform1i(pass->GetRenderTypeUniform(), RT_MODEL));
		}


		DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + MAX_NUM_SHADOW_MAPS));
		DEBUG_OGL(glUniform1i(pass->GetWaterNormalMapUniform(), MAX_NUM_SHADOW_MAPS));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->GetNormalMap()->GetTextureId()));


		if (component->GetEngine()->TextureSamplingSwitched() == true) {
			if (component->GetEngine()->GetTextureSamplingQuality()) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			//component->GetEngine()->DoSwitchTextureSamplingQuality();
		}

		if (component->GetEngine()->MipMappingSwitched() == true) {
			if (component->GetEngine()->GetMipMappingQuality() == 0) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 1) {
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 2) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 3) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 4) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}

			//component->GetEngine()->DoSwitchMipMappingQuality();
		}

		DEBUG_OGL(glActiveTexture(GL_TEXTURE0 + MAX_NUM_SHADOW_MAPS + 1));
		DEBUG_OGL(glUniform1i(pass->GetWaterUVDVMapUniform(), MAX_NUM_SHADOW_MAPS + 1));
		DEBUG_OGL(glBindTexture(GL_TEXTURE_2D, component->GetDuDv()->GetTextureId()));

		if (component->GetEngine()->TextureSamplingSwitched() == true) {
			if (component->GetEngine()->GetTextureSamplingQuality()) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			//component->GetEngine()->DoSwitchTextureSamplingQuality();
		}

		if (component->GetEngine()->MipMappingSwitched() == true) {
			if (component->GetEngine()->GetMipMappingQuality() == 0) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 1) {
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 2) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 3) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			}

			else if (component->GetEngine()->GetMipMappingQuality() == 4) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}

			//component->GetEngine()->DoSwitchMipMappingQuality();
		}

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
		DEBUG_OGL(glBindVertexArray(0));

		float texOffset = component->getTexAlpha();
		float waveOffset = component->getWaveAlpha();

		DEBUG_OGL(glUniform1f(pass->GetWaveOffsetUniform(), waveOffset/1000));
		DEBUG_OGL(glUniform1f(pass->GetTexOffsetUniform(), texOffset));


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


			this->resource->GetMeshes()[0]->renderType = RT_WATER;
		}

		for (auto &mesh : this->resource->GetMeshes())
		{
			GetEngine()->GetRenderPass()->AddOperation(new WaterRenderOperation(mesh, this));
		}
	}

	WaterSurface::WaterSurface() {
		auto mapSize = vec3(2048, 60, 2048);
		this->resource = new HeightMapResource(new TextureResource("textures/water.png"), mapSize, 10, 10);

		this->normalmap = new TextureResource("textures/normalMap.png");
		this->dudv = new TextureResource("textures/dudvmap.jpg");
		
		this->texOffset = nullptr;
		this->waveOffset = nullptr;

		this->movefactor = 0;
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
		return 0;
	}

	float WaterSurface::getWaveAlpha() {
		this->movefactor += 0.003 * this->GetEngine()->GetDeltaTime();
		return this->movefactor;
	}

}