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

	void OpenGLRenderer::Submit(std::unique_ptr<VertexArray>& va)
	{
		va->Bind();
		glDrawElements(GL_TRIANGLES, va->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}
}