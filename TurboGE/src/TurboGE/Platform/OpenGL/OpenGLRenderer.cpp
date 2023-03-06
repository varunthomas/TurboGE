#include"tgepch.h"
#include"OpenGLRenderer.h"
#include"OpenGLShader.h"
#include<glad/glad.h>
namespace TurboGE
{

	OpenGLRenderer::OpenGLRenderer()
	{

	}

	void OpenGLRenderer::setClearColor()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1);
	}

	void OpenGLRenderer::setViewPort(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::Clear()
	{
		//NOT SAME IN CHERNO
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderer::StartScene(const OrthographicCamera& camera)
	{
		m_viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void OpenGLRenderer::Submit(std::unique_ptr<Shader>& shader, std::unique_ptr<VertexArray>& va, glm::mat4& transform)
	{
		shader->Bind();
		dynamic_cast<OpenGLShader*>(shader.get())->uploadUniformMat4("u_ViewProjection", m_viewProjectionMatrix);
		dynamic_cast<OpenGLShader*>(shader.get())->uploadUniformMat4("u_Transform", transform);

		va->Bind();
		glDrawElements(GL_TRIANGLES, va->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

	}
}