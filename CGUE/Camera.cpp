#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"
#include "Pass.h"
#include "RenderPass.h"
#include <SDL.h>
#include <ratio>
#include <iostream>

namespace Engine {
	const Camera CameraClass;

	void CameraRenderOperation::Execute()
	{
		auto component = static_cast<Camera*>(this->GetComponent());

		if (component->updatedTransformation)
		{

			auto pos = component->GetTransformation()->GetAbsolutePosition();
			component->viewMatrix = lookAt(pos, component->lookAtVector, component->upVector);
			component->frustumChanged = true;
			component->updatedTransformation = false;
		}

		auto oldMainCamera = component->GetEngine()->GetMainCamera();
		component->GetEngine()->SetMainCamera(component);

		if (component->frustumChanged)
		{
			component->RefreshFrustum();
		}

		if (component->frameBuffer)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			DEBUG_OGL(glBindFramebuffer(GL_FRAMEBUFFER, component->frameBuffer));
		}
		DEBUG_OGL(glViewport(0, 0, component->width, component->height));

		component->cameraPass->DoPass();

		if (component->frameBuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		component->GetEngine()->SetMainCamera(oldMainCamera);
	}

	int CameraRenderOperation::GetPriority()
	{
		if (GetComponent()->GetEngine()->GetMainCamera() == GetComponent())
		{
			return MAIN_CAMERA_PRIORITY; // main camera should have very high priority
		}
		// otherwise default
		return Operation::GetPriority();
	}

	Camera::Camera()
	{
		// dont use this constructor
	}

	Camera::Camera(float fov, float near, float far, int width, int height)
	{
		this->width = width;
		this->height = height;
		this->frameBuffer = 0;
		this->texture = 0;
		this->cameraPass = nullptr;
		this->upVector = vec3(0.0, 1.0, 0.0);
		this->renderingEnabled = true;
		this->fov = fov;
		this->near = near;
		this->far = far;
		this->ratio = float(width) / float(height);
		this->frustumChanged = true;
		this->projectionMatrix = perspective(radians(fov), ratio, near, far);
		this->hudEnabled = true;
		this->clippingEnabled = false;
		this->cameraMode = CM_NORMAL; 
		this->depthTexture = -1;
		this->depthBuffer = -1;
		this->isRefraction = false;
		this->isReflection = false;
		this->updatedTransformation = false;



		auto tang = float(tan(0.5*radians(fov)));
		nh = near*tang;
		nw = nh*ratio;
		fh = far*tang;
		fw = fh*ratio;

	}

	Camera::~Camera()
	{
	}

	void Camera::RenderingEnabled(bool enabled)
	{
		this->renderingEnabled = enabled;
	}

	void Camera::SetCameraMode(CameraMode mode)
	{
		this->cameraMode = mode;
	}

	void Camera::SetCameraPass(Pass* pass)
	{
		this->cameraPass = pass;
	}

	void Camera::SetLookAtVector(vec3 lookAt)
	{
		this->lookAtVector = lookAt;
		this->frustumChanged = true;
		this->TransformationUpdated();
	}

	void Camera::SetUpVector(vec3 upVector)
	{
		this->upVector = upVector;
		this->frustumChanged = true;
	}

	vec3 Camera::GetLookAtVector() const
	{
		return this->lookAtVector;
	}

	mat4x4 Camera::GetViewMatrix() const
	{
		return this->viewMatrix;
	}

	mat4x4 Camera::GetProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	mat4x4 Camera::GetHudProjectionMatrix() const
	{
		return this->hudProjectionMatrix;
	}

	GLuint Camera::GetTextureId()
	{
		return this->texture;
	}

	GLuint Camera::GetDepthTextureId() const
	{
		return depthTexture;
	}

	int Camera::GetWidth()
	{
		return this->width;
	}

	int Camera::GetHeight()
	{
		return this->height;
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		if (!renderingEnabled)
		{
			return;
		}

		if (this->cameraPass == nullptr)
		{
			this->cameraPass = GetEngine()->GetRenderPass();
		}
		DEBUG_OGL();
		
		switch(this->cameraMode)
		{
		case CM_DEPTH: {
			// create depth map FBO
			glGenFramebuffers(1, &this->frameBuffer);
			glGenTextures(1, &this->texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

			glDrawBuffer(GL_NONE);
			//glReadBuffer(GL_NONE);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			break;
		}
		case CM_REFLECTION: {
			glGenFramebuffers(1, &this->frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			DEBUG_OGL();

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
				0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				texture, 0);
			DEBUG_OGL();


			glGenRenderbuffers(1, &depthBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width,
				height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, depthBuffer);

			auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				RaiseEngineError("Incomplete Framebuffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
			break;
		case CM_REFRACTION: {
			glGenFramebuffers(1, &this->frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			DEBUG_OGL();

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
				0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				texture, 0);
			DEBUG_OGL();

			glGenTextures(1, &depthTexture);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				depthTexture, 0);


			auto status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
			if (status != GL_FRAMEBUFFER_COMPLETE) {
				RaiseEngineError("Incomplete Framebuffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			break;
		}
		case CM_NORMAL:
			break;
		default:
			RaiseEngineError("Unknown mode");
		}

		if (isReflection)
		{
			GetEngine()->GetRenderPass()->SetReflectionTexture(GetTextureId());
		}
		if (isRefraction)
		{
			GetEngine()->GetRenderPass()->SetRefractionTexture(GetTextureId());
		}

		DEBUG_OGL();

		GetEngine()->GetCameraPass()->AddOperation(new CameraRenderOperation(this));
	}

	vec3 Camera::GetUpVector() const
	{
		return this->upVector;

	}

	bool Camera::IsClippingEnabled() const
	{
		return clippingEnabled;
	}

	vec4 Camera::GetClippingPlane() const
	{
		return clippingPlane;
	}

	void Camera::TransformationUpdated()
	{
		updatedTransformation = true;
	}

	void Camera::RefreshFrustum()
	{
		auto p = GetTransformation()->GetAbsolutePosition();
		auto Z = normalize(p - lookAtVector);
		auto X = normalize(cross(upVector, Z));
		auto Y = cross(Z, X);

		auto nc = p - Z*near;
		auto fc = p - Z*far;

		auto ntl = nc + Y*nh - X*nw;
		auto ntr = nc + Y*nh + X*nw;
		auto nbl = nc - Y*nh - X*nw;
		auto nbr = nc - Y*nh + X*nw;

		auto ftl = fc + Y * fh - X * fw;
		auto ftr = fc + Y * fh + X * fw;
		auto fbl = fc - Y * fh - X * fw;
		auto fbr = fc - Y * fh + X * fw;

		frustumPlanes[VFC_TOP].set3Points(ntr, ntl, ftl);
		frustumPlanes[VFC_BOTTOM].set3Points(nbl, nbr, fbr);
		frustumPlanes[VFC_LEFT].set3Points(ntl, nbl, fbl);
		frustumPlanes[VFC_RIGHT].set3Points(nbr, ntr, fbr);
		frustumPlanes[VFC_NEARP].set3Points(ntl, ntr, nbr);
		frustumPlanes[VFC_FARP].set3Points(ftr, ftl, fbl);


		frustumChanged = false;
	}

	void Camera::SetHudProjectionMatrix(mat4x4 hudMatrix)
	{
		this->hudProjectionMatrix = hudMatrix;
	}

	void Camera::SetClipping(bool enabled, vec4 plane)
	{
		this->clippingEnabled = enabled;
		this->clippingPlane = plane;
	}

	FRUSTUM_COLLISION Camera::PointInFrustum(vec3 &p) const
	{
		auto result = F_INSIDE;
		for (auto i = 0; i < 6; i++) {

			if (frustumPlanes[i].distance(p) < 0)
			{
				return F_OUTSIDE;
			}
		}
		return result;

	}


	FRUSTUM_COLLISION Camera::SphereInFrustum(vec3 &p, float raio) const
	{
		auto result = F_INSIDE;
		float distance;

		for (int i = 0; i < 6; i++) {
			distance = frustumPlanes[i].distance(p);
			if (distance < -raio)
			{
				return F_OUTSIDE;
			}
			else if (distance < raio)
			{
				result = F_INTERSECT;
			}
		}
		return(result);

	}


	FRUSTUM_COLLISION Camera::BoxInFrustum(AABox &b) {
		auto result = F_INSIDE;

		for (auto i = 0; i < 6; i++) {

			if (frustumPlanes[i].distance(b.getVertexP(frustumPlanes[i].normal)) < 0)
			{
				return F_OUTSIDE;
			}
			else if (frustumPlanes[i].distance(b.getVertexN(frustumPlanes[i].normal)) < 0)
			{
				result = F_INTERSECT;
			}
		}
		return result;

	}

	void Camera::SetHudEnabled(bool isEnabled)
	{
		this->hudEnabled = isEnabled;
	}

	bool Camera::IsHudEnabled() const
	{
		return this->hudEnabled;
	}

	void Camera::SetAsReflectionCamera(bool reflection)
	{
		this->isReflection = reflection;
	}

	void Camera::SetAsRefractionCamera(bool refraction)
	{
		this->isRefraction = refraction;
	}

	float Camera::GetFar() const
	{
		return far;
	}
}
