#pragma once
#include "Component.h"
#include "glew/glew.h"
#include "Operation.h"
#include "Model.h"

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

	public:

		WaterSurface();
		WaterSurface(int size);
		~WaterSurface();

		HeightMapResource *GetResource() const;

		void Init() override;
		void Wire() override;
	};

}
