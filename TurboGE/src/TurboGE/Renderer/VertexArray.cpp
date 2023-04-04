#include"tgepch.h"
#include"VertexArray.h"
#include"TurboGE/Platform/OpenGL/OpenGLVertexArray.h"

namespace TurboGE
{
	std::unique_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_unique<OpenGLVertexArray>();
	}
}