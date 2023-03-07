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
		OpenGLShader(const std::string&);
		~OpenGLShader();
		void parseFile(const std::string&, std::string&, std::string&);
		void CompileProgram(const std::string&, const std::string&);
		void Bind() override;
		void Unbind() override;


		void UploadUniformInt(const std::string&, int);
		void uploadUniformMat4(const std::string&, const glm::mat4&);
		void uploadUniformFloat3(const std::string&, const glm::vec3&);
		void uploadUniformFloat4(const std::string&, const glm::vec4&);

		void SetMat4(const std::string&, const glm::mat4) override;
		void SetFloat4(const std::string&, const glm::vec4) override;
		void SetFloat3(const std::string&, const glm::vec3) override;
		void SetInt(const std::string&, int) override;
	};
}