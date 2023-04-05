#include"tgepch.h"
#include"FrameBuffer.h"
#include"TurboGE/Platform/OpenGL/OpenGLFrameBuffer.h"
namespace TurboGE
{
	std::unique_ptr<FrameBuffer> FrameBuffer::Create(FrameBufferSpec& fbSpec)
	{
		return std::make_unique<OpenGLFrameBuffer>(fbSpec);
	}
}