#pragma once
#include<string>
#include<glm/glm.hpp>

namespace TurboGE
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Shader* Create(const std::string&);
		static Shader* Create(const std::string&, const std::string&);

		virtual void SetMat4(const std::string&, glm::mat4) = 0;
		virtual void SetFloat4(const std::string&, glm::vec4) = 0;
		virtual void SetFloat3(const std::string&, glm::vec3) = 0;
		virtual void SetInt(const std::string&, int) = 0;
		virtual void SetIntArray(const std::string&, int*, uint32_t) = 0;
	};
}