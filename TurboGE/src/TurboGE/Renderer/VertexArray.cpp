#include"tgepch.h"
#include"VertexArray.h"
#include"TurboGE/Platform/OpenGL/OpenGLVertexArray.h"

namespace TurboGE
{
	VertexArray* VertexArray::Create()
	{
		return new OpenGLVertexArray();
	}
}