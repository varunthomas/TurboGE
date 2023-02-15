#pragma once

namespace TurboGE
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static VertexBuffer* Create(float*, uint32_t);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static IndexBuffer* Create(unsigned int*, uint32_t);
	};
}