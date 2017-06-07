#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"
#include "Model.h"
#include "TextureResource.h"

namespace Engine {

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

		WaterSurface();
		WaterSurface(int size);
		~WaterSurface();

		HeightMapResource *GetResource() const;
		TextureResource *GetNormalMap() const;
		TextureResource *GetDuDv() const;

		void Init() override;
		void Wire() override;
	};

}
