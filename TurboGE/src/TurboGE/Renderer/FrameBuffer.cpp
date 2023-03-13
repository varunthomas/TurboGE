#include"tgepch.h"
#include"FrameBuffer.h"
#include"TurboGE/Platform/OpenGL/OpenGLFrameBuffer.h"
namespace TurboGE
{
	std::unique_ptr<FrameBuffer> FrameBuffer::Create(uint32_t width, uint32_t height)
	{
		return std::make_unique<OpenGLFrameBuffer>(width, height);
	}
}