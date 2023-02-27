#include"tgepch.h"
#include"Buffer.h"
#include"TurboGE/Platform/OpenGL/OpenGLBuffer.h"

namespace TurboGE
{

	void VertexLayout::MakeLayout()
	{
		int offset = 0;
		for (auto& attrib : m_attribVec)
		{
			attrib.m_offset += offset; 
			m_stride += typeToStride(attrib.m_type, attrib.m_size);
			offset = m_stride;
		}
	}

	int VertexLayout::typeToStride(AttribType type, int& size)
	{
		switch (type)
		{
		case AttribType::Float2:
			size = 2;
			return 2 * sizeof(float);
		case AttribType::Float3:
			size = 3;
			return 3 * sizeof(float);
		case AttribType::Float4:
			size = 4;
			return 4 * sizeof(float);
		}
		return 0;
	}

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