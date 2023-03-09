#pragma once
#include"Buffer.h"

namespace TurboGE
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void SetLayout(VertexLayout&) = 0;
		virtual VertexLayout& GetLayout() = 0;
		virtual void BindVertexBuffer() = 0;
		virtual void Bind() = 0;
		virtual void DrawCommand(uint32_t count = 0) = 0;

		virtual std::shared_ptr<IndexBuffer>& getIndexBuffer() = 0;
		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>&) = 0;

		static VertexArray* Create();

		
	};
}