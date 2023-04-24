#include"tgepch.h"
#include"OpenGLFrameBuffer.h"
#include<glad/glad.h>

namespace TurboGE
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferSpec& fbSpec)
		:m_Specification{ fbSpec }
	{

		for (auto format : fbSpec.formats)
		{
			if (!isDepthFormat(format))
			{
				m_ColorFormats.emplace_back(format);
			}
			else
			{
				m_DepthFormat = format;
			}
		}
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachment.size(), m_ColorAttachment.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.width, m_Specification.height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool OpenGLFrameBuffer::isDepthFormat(FrameBufferFormat format)
	{
		if (FrameBufferFormat::DEPTH24_STENCIL8 == format)
		{
			return true;
		}
		return false;
	}

	int OpenGLFrameBuffer::GetPixelData(uint32_t attachmentIndex, int mouseX, int mouseY)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::SetFrameSpec(const FrameBufferSpec spec)
	{
		m_Specification.width = spec.width;
		m_Specification.height = spec.height;
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachment.size(), m_ColorAttachment.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachment.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


		//COLOR ATTACHMENT
		m_ColorAttachment.resize(m_ColorFormats.size());
		glCreateTextures(GL_TEXTURE_2D, m_ColorAttachment.size(), m_ColorAttachment.data());
		for (size_t i{ 0 }; i < m_ColorAttachment.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachment[i]);

			switch (m_ColorFormats[i])
			{
			case FrameBufferFormat::RGBA8:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachment[i], 0);

				break;
			case FrameBufferFormat::RED_INT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_Specification.width, m_Specification.height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorAttachment[i], 0);
				break;
			}
		}

		//DEPTH ATTACHMENT
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		switch (m_DepthFormat)
		{
		case FrameBufferFormat::DEPTH24_STENCIL8:
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
			break;
		}

		//DRAW APPLICABLE FOR DEPTH ONLY OR MORE THAN ONE COLOR ATTACHMENT
		if (m_ColorAttachment.size() > 1)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachment.size(), buffers);
		}
		else if (m_ColorAttachment.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::ClearEntityAttachment(uint32_t index, int value)
	{
		glClearTexImage(m_ColorAttachment[index], 0,
			GL_RED_INTEGER, GL_INT, &value);
	}
}