#pragma once
#include"TurboGE/Renderer/Shader.h"
#include<glm/glm.hpp>

namespace TurboGE
{
	class OpenGLShader : public Shader
	{
		uint32_t m_RendererID;
	public:
		OpenGLShader(const std::string&, const std::string&);
		~OpenGLShader();
		void Bind() override;
		void Unbind() override;

		void UploadUniformInt(const std::string&, int);
		void uploadUniformMat4(const std::string&, const glm::mat4&);
		void uploadUniformFloat3(const std::string&, const glm::vec3&);
	};
}