#include"tgepch.h"
#include"TurboGE/Platform/OpenGL/OpenGLTexture.h"

namespace TurboGE
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		return std::make_shared<OpenGLTexture2D>(path);
	}

	std::shared_ptr<Texture2D> Texture2D::Create()
	{
		return std::make_shared<OpenGLTexture2D>();
	}
}