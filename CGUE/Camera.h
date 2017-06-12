#pragma once
#include "Transformation.h"
#include "glew/glew.h"
#include "Operation.h"
#include "TextureRenderable.h"
#include "Frustum.h"

namespace Engine {
	enum {
		VFC_TOP = 0,
		VFC_BOTTOM,
		VFC_LEFT,
		VFC_RIGHT,
		VFC_NEARP,
		VFC_FARP
	};


	enum FRUSTUM_COLLISION
	{
		F_OUTSIDE,
		F_INTERSECT,
		F_INSIDE
	};


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
		bool frustumChanged;

		bool renderingEnabled;

		mat4x4 projectionMatrix;
		mat4x4 hudProjectionMatrix;
		vec3 lookAtVector;
		vec3 upVector;

		GLuint depthMapFbo;
		GLuint depthMap;

		GLuint renderFbo;
		GLuint texture;
		GLuint renderbuffer;

		bool r2t;
		bool renderImage;
		Pass* cameraPass;
		float fov;
		float near;
		float far;
		float ratio;

		Plane frustumPlanes[6];
		float nh;
		float nw;
		float fh;
		float fw;
	public:
		mat4x4 viewMatrix;

		//explicit Camera(float fov = 45.0f, float near = 0.1f, float far=100.0f, int width=640, int height=480, bool ortho = false);
		explicit Camera();
		explicit Camera(float fov, float near, float far, int width, int height);
		~Camera();

		void RenderingEnabled(bool enabled);

		virtual void EnableRender2Texture();
		virtual void EnableRenderImage();
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
		void RefreshFrustum();

		FRUSTUM_COLLISION PointInFrustum(vec3 &p) const;
		FRUSTUM_COLLISION SphereInFrustum(vec3 &p, float raio) const;
		FRUSTUM_COLLISION Camera::BoxInFrustum(AABox &b);
	};

	extern const Camera CameraClass;
}
