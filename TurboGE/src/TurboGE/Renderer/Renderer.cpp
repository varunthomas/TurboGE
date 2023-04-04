#include"tgepch.h"
#include"Renderer.h"
#include"TurboGE/Platform/OpenGL/OpenGLRenderer.h"
namespace TurboGE
{
	std::unique_ptr<Renderer> Renderer::Create()
	{
		return std::make_unique<OpenGLRenderer>();
	}
}