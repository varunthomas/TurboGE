#pragma once
#include<memory>
namespace TurboGE
{
	enum class FrameBufferFormat
	{
		RGBA8 = 1,
		RED_INT,

		DEPTH24_STENCIL8
	};


	struct FrameBufferSpec
	{
		uint32_t width, height;
		std::vector<FrameBufferFormat> formats;
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
		virtual void SetFrameSpec(const FrameBufferSpec) = 0;
		virtual uint32_t GetID(uint32_t index = 0) = 0;
		virtual int GetPixelData(uint32_t, int, int) = 0;
		virtual void ClearEntityAttachment(uint32_t, int) = 0;
	};
}