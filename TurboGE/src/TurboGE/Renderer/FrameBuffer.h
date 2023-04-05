#pragma once
#include<memory>
namespace TurboGE
{
	struct FrameBufferSpec
	{
		uint32_t width, height;
	};
	class FrameBuffer
	{
	public:
		static std::unique_ptr<FrameBuffer> Create(FrameBufferSpec&);
		virtual ~FrameBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		//virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual const FrameBufferSpec& GetSpecification() const = 0;
		virtual void SetFrameSpec(const FrameBufferSpec&) = 0;
		virtual uint32_t GetID() = 0;
	};
}