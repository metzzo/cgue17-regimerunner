#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"
#include "Model.h"
#include "TextureResource.h"
#include "Timer.h"

namespace Engine {

	class Timer;

	class WaterRenderOperation : public Operation {
		Mesh *mesh;
	public:
		explicit WaterRenderOperation(Mesh *mesh, Component* component) : Operation(component) {
			this->mesh = mesh;
		}
		void Execute() override;
	};

	class WaterSurface : public Component {

		HeightMapResource* resource;
		TextureResource* normalmap;
		TextureResource* dudv;

	public:

		Timer* texOffset;
		Timer* waveOffset;

		WaterSurface();
		WaterSurface(int size);
		~WaterSurface();

		HeightMapResource *GetResource() const;
		TextureResource *GetNormalMap() const;
		TextureResource *GetDuDv() const;

		float getWaveAlpha();
		float getTexAlpha();

		void Init() override;
		void Wire() override;
	};

}
