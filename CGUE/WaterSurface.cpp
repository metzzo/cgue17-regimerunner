#include "WaterSurface.h"
#include "Entity.h"
#include "Camera.h"
#include "GameEngine.h"
#include "HeightMapResource.h"
#include "TextureResource.h"
#include "RenderPass.h"

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

		GLfloat directionArr[8][8];

		for (int i = 0; i < 4; i++) {
			float amplitude = 0.5f / (i + 1);
			GLint loc = pass->GetArrayUniformLocation(i, "amplitude");
			DEBUG_OGL(glUniform1f(loc, amplitude));

			float wavelength = 8 * M_PI / (i + 1);
			loc = pass->GetArrayUniformLocation(i, "wavelength");
			DEBUG_OGL(glUniform1f(loc, wavelength));

			float speed = 1.0f + 2 * i;
			loc = pass->GetArrayUniformLocation(i, "speed");
			DEBUG_OGL(glUniform1f(loc, speed));

			float min = -M_PI / 3;
			float max = M_PI / 3;
			double n = (double)rand() / (double)RAND_MAX;
			double angle = min + n * (max - min);
			auto dir = glm::vec2(sin(angle), cos(angle));
			loc = pass->GetArrayUniformLocation(i, "direction");
			DEBUG_OGL(glUniform2fv(loc,1, &dir[0]));
		}

		DEBUG_OGL(glUniform1i(pass->GetWaterHeightUniform(), 5));
		DEBUG_OGL(glUniform1i(pass->GetNumberOfWavesUniform(), 4));

		//DEBUG_OGL(glUniform4fv(pass->GetAmplitudeUniform(),8, &amplitudeArr[0]));
		//DEBUG_OGL(glUniform4fv(pass->GetWaveLengthUniform(), 4, waveArr));
		//DEBUG_OGL(glUniform4fv(pass->GetSpeedUniform(), 4, speedArr));
		//DEBUG_OGL(glUniform4fv(pass->GetDirectionUniform(), 4, (GLfloat *)&directionArr));

		DEBUG_OGL(glBindVertexArray(mesh->VAO));
		DEBUG_OGL(glDrawElements(mesh->mode, mesh->indices.size(), GL_UNSIGNED_INT, nullptr));
		DEBUG_OGL(glBindVertexArray(0));

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
		}

		for (auto &mesh : this->resource->GetMeshes())
		{
			GetEngine()->GetRenderPass()->AddOperation(new WaterRenderOperation(mesh, this));
		}
	}

	WaterSurface::WaterSurface() {
		auto mapSize = vec3(1024, 40, 1024);
		this->resource = new HeightMapResource("textures/water.jpg", mapSize, 200, 200);
	}

	WaterSurface::WaterSurface(int size) {
	
	}

	WaterSurface::~WaterSurface() {
	
	}

	HeightMapResource * WaterSurface::GetResource() const
	{
		return this->resource;
	}

}