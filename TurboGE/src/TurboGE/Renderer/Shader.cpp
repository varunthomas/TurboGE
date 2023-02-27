#include"tgepch.h"
#include"TurboGE/Platform/OpenGL/OpenGLShader.h"

namespace TurboGE
{

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		return new OpenGLShader(vertexSrc, fragmentSrc);
	}

}