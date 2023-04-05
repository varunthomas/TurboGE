#pragma once
#include"TurboGE/Renderer/FrameBuffer.h"

namespace TurboGE
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
		//uint32_t m_width, m_height;
		uint32_t m_RendererID, m_DepthAttachment, m_ColorAttachment;
		FrameBufferSpec m_Specification;
	public:
		OpenGLFrameBuffer(FrameBufferSpec&);
		~OpenGLFrameBuffer();

		void Bind() override;
		void Unbind() override;

		//void Resize(uint32_t width, uint32_t height) override;
		void SetFrameSpec(const FrameBufferSpec& fbSpec) override { m_Specification = fbSpec; }
		const FrameBufferSpec& GetSpecification() const override { return m_Specification; }
		uint32_t GetID() override { return m_ColorAttachment; }
	};
}