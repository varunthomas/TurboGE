#pragma once
#include"TurboGE/Renderer/Buffer.h"

namespace TurboGE
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		uint32_t m_RendererID;
		//VertexLayout m_layout;
	public:
		OpenGLVertexBuffer(float*, uint32_t);
		~OpenGLVertexBuffer();

		//void SetLayout(VertexLayout& vl) override { m_layout = vl; }
		//VertexLayout GetLayout() override { return m_layout; }

		void Bind() override;
		void Unbind() override;

	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
		uint32_t m_RendererID;
		unsigned int m_count;
	public:
		OpenGLIndexBuffer(unsigned int*, uint32_t);
		~OpenGLIndexBuffer();

		void Bind() override;
		void Unbind() override;
		unsigned int getCount() override { return m_count; }

	};

}