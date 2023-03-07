#include"tgepch.h"
#include"TurboGE/Platform/OpenGL/OpenGLTexture.h"

namespace TurboGE
{
	std::unique_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		return std::make_unique<OpenGLTexture2D>(path);
	}

	std::unique_ptr<Texture2D> Texture2D::Create()
	{
		return std::make_unique<OpenGLTexture2D>();
	}
}