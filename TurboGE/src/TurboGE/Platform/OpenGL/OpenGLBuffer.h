#pragma once
#include"TurboGE/Renderer/Buffer.h"

namespace TurboGE
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
		uint32_t m_RendererID;
	public:
		OpenGLVertexBuffer(float*, uint32_t);
		OpenGLVertexBuffer(uint32_t);
		~OpenGLVertexBuffer();

		void SetBatchData(uint32_t, const void*);

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