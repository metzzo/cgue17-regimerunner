#include "Camera.h"
#include "Entity.h"
#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glew/glew.h"

namespace Engine {
	const Camera CameraClass;

	GLuint quadVAO = 0;
	GLuint quadVBO;
	void RenderQuad()
	{
		if (quadVAO == 0)
		{
			GLfloat quadVertices[] = {
				// Positions        // Texture Coords
				-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
				1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
				1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			};
			// Setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}


	void CameraRenderOperation::Execute()
	{
		auto component = static_cast<Camera*>(this->GetComponent());

		auto oldMainCamera = component->GetEngine()->GetMainCamera();
		component->GetEngine()->SetMainCamera(component);

		glViewport(0, 0, component->width, component->height);
		if (component->depthMapFbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, component->depthMapFbo);
		}
		if (component->renderOperation == DEPTH_PASS_OPERATION) {
			glClear(GL_DEPTH_BUFFER_BIT);
		} else
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}
		// TODO: only draw objects that could potentially visible for the camera
		component->GetEngine()->ProcessQueue(component->renderOperation);
		if (component->depthMapFbo)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			/*glViewport(0, 0, 640, 480);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(component->debugShader->GetProgramId());
			glUniform1f(glGetUniformLocation(component->debugShader->GetProgramId(), "near_plane"), 1.0);
			glUniform1f(glGetUniformLocation(component->debugShader->GetProgramId(), "far_plane"), 10.0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, component->depthMap);
			RenderQuad();*/
		}

		component->GetEngine()->SetMainCamera(oldMainCamera);
	}

	OPERATION_TYPE CameraRenderOperation::GetOperationType()
	{
		return CAMERA_PASS_OPERATION;
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

	Camera::Camera(float fov, float near, float far, int width, int height, bool ortho)
	{
		this->fov = fov;
		this->near = near;
		this->far = far;
		this->width = width;
		this->height = height;
		this->ortho = ortho;
		this->depthMapFbo = 0;
		this->depthMap = 0;
		this->textureWidth = 0;
		this->textureHeight = 0;
		this->renderOperation = RENDER_PASS_OPERATION;
		this->debugShader = nullptr;
	}

	Camera::~Camera()
	{
	}

	float Camera::GetFov() const
	{
		return this->fov;
	}

	float Camera::GetFar() const
	{
		return this->far;
	}

	float Camera::GetNear() const
	{
		return this->near;
	}

	void Camera::EnableRender2Texture(int textureWidth, int textureHeight, OPERATION_TYPE renderOperation)
	{
		this->renderOperation = renderOperation;
		this->textureWidth = textureWidth;
		this->textureHeight = textureHeight;
	}

	mat4x4 Camera::GetProjectionViewMatrix() const
	{
		return this->projectionMatrix * this->GetEntity()->GetTransformation()->GetAbsoluteMatrix();
	}

	mat4x4 Camera::GetProjectionMatrix() const
	{
		return this->projectionMatrix;
	}

	GLuint Camera::GetTexture() const
	{
		return this->depthMap;
	}

	void Camera::Wire()
	{
	}

	void Camera::Init()
	{
		if (ortho)
		{
			projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, this->near, this->far);
		}
		else {
			auto engine = this->GetEntity()->GetEngine();
			auto ratio = float(engine->GetScreenWidth()) / float(engine->GetScreenHeight());
			projectionMatrix = perspective(radians(fov), ratio, near, far);
		}

		if (this->renderOperation == DEPTH_PASS_OPERATION)
		{
			// create depth map FBO
			glGenFramebuffers(1, &this->depthMapFbo);
			glGenTextures(1, &this->depthMap);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				textureWidth, textureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		this->debugShader = new Shader("materials/debug_depth_material.vert", "materials/debug_depth_material.frag");
		this->debugShader->Init();

		GetEngine()->AddOperation(new CameraRenderOperation(this));
	}
}
