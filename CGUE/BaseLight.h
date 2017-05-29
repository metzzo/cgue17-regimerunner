#pragma once
#include "Component.h"
#include "glm/glm.hpp"

using namespace  glm;

namespace Engine {
	class BaseLight :
		public Engine::Component
	{
		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;

		void RefreshRenderPass() const;
	public:
		BaseLight();
		~BaseLight();

		void AttachedToEntity() override;

		void SetAmbient(vec3 ambient);
		void SetDiffuse(vec3 diffuse);
		void SetSpecular(vec3 specular);

		void SetConstant(float val);
		void SetLinear(float val);
		void SetQuadratic(float val);

		vec3 GetAmbient() const;
		vec3 GetDiffuse() const;
		vec3 GetSpecular() const;

		float GetConstant() const;
		float GetLinear() const;
		float GetQuadratic() const;
	};
}