#pragma once
#include"TurboGE/Renderer/VertexArray.h"

namespace TurboGE
{
	class OpenGLVertexArray : public VertexArray
	{
		unsigned int m_RendererID;
		VertexLayout m_layout;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		void SetLayout(VertexLayout& vl) override { m_layout = vl; }
		VertexLayout& GetLayout() override { return m_layout; }

		void DrawCommand(uint32_t count = 0) override;

		std::shared_ptr<IndexBuffer>& getIndexBuffer() override { return m_IndexBuffer; }
		void setIndexBuffer(std::shared_ptr<IndexBuffer>&) override;

		void Bind() override;
		void BindVertexBuffer(std::shared_ptr<VertexBuffer>&) override;
	};
}