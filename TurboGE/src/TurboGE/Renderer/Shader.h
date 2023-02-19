#pragma once
#include<string>
#include<glm/glm.hpp>

namespace TurboGE
{
	class Shader
	{
		uint32_t m_RendererID;
	public:
		Shader(const std::string&, const std::string&);
		~Shader();
		void Bind();
		void uploadUniformMat4(const std::string&, const glm::mat4&);
		void Unbind();
	};
}