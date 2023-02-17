#include"tgepch.h"
#include"Renderer.h"
#include"TurboGE/Platform/OpenGL/OpenGLRenderer.h"
namespace TurboGE
{
	Renderer* Renderer::Create()
	{
		return new OpenGLRenderer();
	}
}