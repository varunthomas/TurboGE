#include"tgepch.h"
#include"OpenGLVertexArray.h"
#include"TurboGE/Renderer/Buffer.h"
#include<glad/glad.h>

namespace TurboGE
{

	GLenum typeToEnum(AttribType type)
	{
		switch (type)
		{
		case AttribType::Float:		return GL_FLOAT;
		case AttribType::Float2:	return GL_FLOAT;
		case AttribType::Float3:	return GL_FLOAT;
		case AttribType::Float4:	return GL_FLOAT;
		case AttribType::Int:		return GL_INT;
		}
		TURBO_ASSERT("Invalid Attrib type", 0);
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		//glCreateVertexArrays(1, &m_RendererID);
		glGenVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::BindVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		VertexLayout& layout = GetLayout();
		for (auto& attrib : layout.m_attribVec)
		{
			if (attrib.m_entityID)
			{
				glEnableVertexAttribArray(attrib.m_index);
				glVertexAttribIPointer(attrib.m_index, attrib.m_size, typeToEnum(attrib.m_type), layout.getStride(), (const void*)attrib.m_offset);

			}
			else
			{
				glEnableVertexAttribArray(attrib.m_index);
				glVertexAttribPointer(attrib.m_index, attrib.m_size, typeToEnum(attrib.m_type), attrib.m_normalized, layout.getStride(), (const void*)attrib.m_offset);
			}
		}
	}

	void OpenGLVertexArray::DrawCommand(uint32_t count)
	{
		if (count == 0)
		{
			glDrawElements(GL_TRIANGLES, this->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		}
	}

	void OpenGLVertexArray::DrawLineCommand(uint32_t count)
	{
		glDrawArrays(GL_LINES, 0, count);
	}

	void OpenGLVertexArray::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer>& ib)
	{
		glBindVertexArray(m_RendererID);
		ib->Bind();
		m_IndexBuffer = ib;
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
}