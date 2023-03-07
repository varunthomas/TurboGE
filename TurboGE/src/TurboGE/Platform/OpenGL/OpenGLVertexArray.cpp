#include"tgepch.h"
#include"OpenGLVertexArray.h"
#include"TurboGE/Renderer/Buffer.h"
#include<glad/glad.h>
#include"TurboGE/Logger.h"

namespace TurboGE
{

	GLenum typeToEnum(AttribType type)
	{
		switch (type)
		{
		case AttribType::Float2:	return GL_FLOAT;
		case AttribType::Float3:	return GL_FLOAT;
		case AttribType::Float4:	return GL_FLOAT;
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::BindVertexBuffer()
	{
		VertexLayout& layout = GetLayout();
		for (auto& attrib : layout.m_attribVec)
		{
			glEnableVertexAttribArray(attrib.m_index);
			glVertexAttribPointer(attrib.m_index, attrib.m_size, typeToEnum(attrib.m_type), attrib.m_normalized, layout.getStride(), (const void*)attrib.m_offset);
		}
	}

	void OpenGLVertexArray::DrawCommand()
	{
		glDrawElements(GL_TRIANGLES, this->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
}