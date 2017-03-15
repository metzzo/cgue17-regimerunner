#pragma once
#include "Transformation.h"
#include "glew/glew.h"
#include "Operation.h"
#include "Material.h"

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
		OPERATION_TYPE GetOperationType() override;
		int GetPriority() override;
	};


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
		bool ortho;

		GLuint depthMapFbo;
		OPERATION_TYPE renderOperation;
		int textureWidth;
		int textureHeight;
		Shader* debugShader	;
	public:
		GLuint depthMap; // TODO: not public

		explicit Camera(float fov = 45.0f, float near = 0.1f, float far=100.0f, int width=640, int height=480, bool ortho = false);
		~Camera();

		float GetFov() const;
		float GetFar() const;
		float GetNear() const; 
		virtual void EnableRender2Texture(int textureWidth, int textureHeight, OPERATION_TYPE renderOperation);

		mat4x4 GetProjectionViewMatrix() const;
		mat4x4 GetProjectionMatrix() const;
		GLuint GetTexture() const;

		void Wire() override;
		void Init() override;
	};

	extern const Camera CameraClass;
}
