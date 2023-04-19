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
		case AttribType::Float:
			size = 1;
			return 1 * sizeof(float);
		case AttribType::Float2:
			size = 2;
			return 2 * sizeof(float);
		case AttribType::Float3:
			size = 3;
			return 3 * sizeof(float);
		case AttribType::Float4:
			size = 4;
			return 4 * sizeof(float);
		case AttribType::Int:
			size = 1;
			return 1 * sizeof(int);
		}
		return 0;
	}

	/////FOR NOW ONLY OPENGL IS USED
	std::unique_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return std::make_unique<OpenGLVertexBuffer>(vertices, size);
		//return new OpenGLVertexBuffer(vertices, size);
	}

	std::unique_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return std::make_unique<OpenGLVertexBuffer>(size);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* indices, uint32_t size)
	{
		return std::make_shared<OpenGLIndexBuffer>(indices, size);
	}
}