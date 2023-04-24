#pragma once
#include"TurboGE/Renderer/Shader.h"
#include<glm/glm.hpp>
#include<array>

namespace TurboGE
{
	constexpr uint8_t shaderNum = 2; //VERTEX, FRAGMENT

	class OpenGLShader : public Shader
	{
		uint32_t m_RendererID;
		std::string m_Path;
		
		std::array < std::vector<uint32_t>, 2> openGLShaderBin;
		std::array < std::vector<uint32_t>, 2> vulkanShaderBin;
		std::array<std::string, 2> openGLText;

		std::array<std::string, 2> binFileVulkan{ ".vulkan.vert.bin", ".vulkan.frag.bin" };
		std::array<std::string, 2> binFileOpenGL{ ".openGL.vert.bin", ".openGL.frag.bin" };

		bool isContentSame(std::ifstream&, std::ifstream&);
		void parseFile(const std::string&, std::string&, std::string&);
		bool isBinariesPresent();
		void CompileVulkan(const std::vector<std::string>&);
		void CompileOpenGL();
		void CreateProgram();
	public:
		OpenGLShader(const std::string&);
		~OpenGLShader();

		void Bind() override;
		void Unbind() override;


		void UploadUniformInt(const std::string&, int);
		void UploadUniformIntArray(const std::string&, int*, uint32_t);
		void uploadUniformMat4(const std::string&, const glm::mat4&);
		void uploadUniformFloat3(const std::string&, const glm::vec3&);
		void uploadUniformFloat4(const std::string&, const glm::vec4&);

		void SetMat4(const std::string&, const glm::mat4) override;
		void SetFloat4(const std::string&, const glm::vec4) override;
		void SetFloat3(const std::string&, const glm::vec3) override;
		void SetInt(const std::string&, int) override;
		void SetIntArray(const std::string&, int*, uint32_t) override;
	};
}