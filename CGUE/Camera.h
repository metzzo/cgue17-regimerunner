#pragma once
#include "Transformation.h"
#include "glew/glew.h"
#include "Operation.h"
#include "TextureRenderable.h"

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
		public Component, public TextureRenderable
	{
		friend CameraRenderOperation;

	protected:
		int width;
		int height;

		bool renderingEnabled;

		mat4x4 projectionMatrix;
		mat4x4 hudProjectionMatrix;
		mat4x4 viewMatrix;
		vec3 lookAtVector;
		vec3 upVector;

		GLuint depthMapFbo;
		GLuint depthMap;
		bool r2t;
		Pass* cameraPass;
	public:

		//explicit Camera(float fov = 45.0f, float near = 0.1f, float far=100.0f, int width=640, int height=480, bool ortho = false);
		explicit Camera();
		explicit Camera(mat4x4 projectionMatrix, int width, int height);
		~Camera();

		void RenderingEnabled(bool enabled);

		virtual void EnableRender2Texture();
		void SetCameraPass(Pass *pass);

		void SetLookAtVector(vec3 lookAt);
		void SetUpVector(vec3 upVector);
		void SetHudProjectionMatrix(mat4x4 hudMatrix);

		vec3 GetLookAtVector() const;
		mat4x4 GetViewMatrix() const;
		mat4x4 GetProjectionMatrix() const;
		mat4x4 GetHudProjectionMatrix() const;
		vec3 GetUpVector() const;

		GLuint GetTextureId() override;
		int GetWidth() override;
		int GetHeight() override;

		void Wire() override;
		void Init() override;
		void TransformationUpdated() override;
	};

	extern const Camera CameraClass;
}
