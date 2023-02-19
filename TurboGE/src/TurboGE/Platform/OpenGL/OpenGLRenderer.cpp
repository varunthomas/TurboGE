#include"tgepch.h"
#include"OpenGLRenderer.h"
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

	void OpenGLRenderer::Clear()
	{
		//NOT SAME IN CHERNO
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::StartScene(const OrthographicCamera& camera)
	{
		m_viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void OpenGLRenderer::Submit(std::unique_ptr<Shader>& shader, std::unique_ptr<VertexArray>& va)
	{
		shader->Bind();
		shader->uploadUniformMat4("u_ViewProjection", m_viewProjectionMatrix);
		va->Bind();
		glDrawElements(GL_TRIANGLES, va->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}