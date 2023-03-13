#pragma once
#include"TurboGE/Renderer/FrameBuffer.h"

namespace TurboGE
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
		uint32_t m_RendererID, m_DepthAttachment, m_ColorAttachment;
	public:
		OpenGLFrameBuffer(uint32_t, uint32_t);
		~OpenGLFrameBuffer();

		void Bind() override;
		void Unbind() override;
		uint32_t GetID() override { return m_ColorAttachment; }
	};
}