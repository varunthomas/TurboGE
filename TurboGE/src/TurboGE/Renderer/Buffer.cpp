#include"tgepch.h"
#include"Buffer.h"
#include"TurboGE/Platform/OpenGL/OpenGLBuffer.h"

namespace TurboGE
{

	/////FOR NOW ONLY OPENGL IS USED
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(vertices, size);
	}

	IndexBuffer* IndexBuffer::Create(unsigned int* indices, uint32_t size)
	{
		return new OpenGLIndexBuffer(indices, size);
	}
}