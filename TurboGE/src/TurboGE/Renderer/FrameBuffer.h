#pragma once
#include<memory>
namespace TurboGE
{
	class FrameBuffer
	{
	public:
		static std::unique_ptr<FrameBuffer> Create(uint32_t, uint32_t);
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetID() = 0;
	};
}