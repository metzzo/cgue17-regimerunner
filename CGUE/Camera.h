#pragma once
#include "Transformation.h"
#include "glew/glew.h"
#include "Operation.h"

namespace Engine {
	const int MAIN_CAMERA_PRIORITY = 1000;

	class CameraRenderOperation : public Operation
	{
	public:
		explicit CameraRenderOperation(Component* component)
			: Operation(component)
		{
		}

		void Execute() override;
		int GetPriority() override;
	};

	class Pass;

	class Camera :
		public Component
	{
		friend CameraRenderOperation;

	protected:
		float fov;
		float far;
		float near;

		int width;
		int height;

		mat4x4 projectionMatrix;
		mat4x4 viewMatrix;
		vec3 lookAtVector;
		vec3 upVector;
		bool ortho;

		GLuint depthMapFbo;
		GLuint depthMap;
		bool r2t;
		Pass* cameraPass;
		bool projectionMatrixSet;
	public:

		explicit Camera(float fov = 45.0f, float near = 0.1f, float far=100.0f, int width=640, int height=480, bool ortho = false);
		explicit Camera(mat4x4 projectionMatrix);
		~Camera();

		float GetFov() const;
		float GetFar() const;
		float GetNear() const; 
		virtual void EnableRender2Texture(int textureWidth, int textureHeight);
		void SetCameraPass(Pass *pass);

		void SetLookAtVector(vec3 lookAt);
		vec3 GetLookAtVector() const;
		mat4x4 GetViewMatrix() const;
		mat4x4 GetProjectionMatrix() const;
		mat4x4 GetProjectionViewMatrix() const;
		GLuint GetTexture() const;

		vec3 GetUpVector() const;

		void Wire() override;
		void Init() override;
		void TransformationUpdated() override;
	};

	extern const Camera CameraClass;
}
