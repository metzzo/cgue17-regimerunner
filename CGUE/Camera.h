#pragma once
#include "Transformation.h"
#include "glew/glew.h"
#include "Operation.h"
#include "TextureRenderable.h"
#include "Frustum.h"
#include "RenderableResource.h"

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

	enum CameraMode
	{
		CM_REFLECTION = 1,
		CM_REFRACTION = 2,
		CM_DEPTH = 3,
		CM_NORMAL = 4
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

		GLuint frameBuffer;
		GLuint texture;
		GLuint depthTexture;
		GLuint depthBuffer;

		bool hudEnabled;
		CameraMode cameraMode;

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

		vec4 clippingPlane;
		bool clippingEnabled;

		bool isReflection;
		bool isRefraction;
	public:
		mat4x4 viewMatrix;

		//explicit Camera(float fov = 45.0f, float near = 0.1f, float far=100.0f, int width=640, int height=480, bool ortho = false);
		explicit Camera();
		explicit Camera(float fov, float near, float far, int width, int height);
		~Camera();

		void RenderingEnabled(bool enabled);

		void SetCameraMode(CameraMode mode);

		void SetCameraPass(Pass *pass);

		void SetLookAtVector(vec3 lookAt);
		void SetUpVector(vec3 upVector);
		void SetHudProjectionMatrix(mat4x4 hudMatrix);
		void SetClipping(bool enabled, vec4 plane);

		vec3 GetLookAtVector() const;
		mat4x4 GetViewMatrix() const;
		mat4x4 GetProjectionMatrix() const;
		mat4x4 GetHudProjectionMatrix() const;
		vec3 GetUpVector() const;
		bool IsClippingEnabled() const;
		vec4 GetClippingPlane() const;

		GLuint GetTextureId() override;
		GLuint GetDepthTextureId() const;
		int GetWidth() override;
		int GetHeight() override;

		void Wire() override;
		void Init() override;
		void TransformationUpdated() override;
		void RefreshFrustum();

		FRUSTUM_COLLISION PointInFrustum(vec3 &p) const;
		FRUSTUM_COLLISION SphereInFrustum(vec3 &p, float raio) const;
		FRUSTUM_COLLISION Camera::BoxInFrustum(AABox &b);
		void SetHudEnabled(bool isEnabled);
		bool IsHudEnabled() const;
		void SetAsReflectionCamera(bool reflection);
		void SetAsRefractionCamera(bool refraction);
		float GetFar() const;
	};

	extern const Camera CameraClass;
}
