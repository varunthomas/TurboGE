#include"tgepch.h"
#include"TurboGE/Platform/OpenGL/OpenGLShader.h"

namespace TurboGE
{

	std::unique_ptr<Shader> Shader::Create(const std::string& path)
	{
		return std::make_unique<OpenGLShader>(path);
	}
}