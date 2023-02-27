#pragma once
#include<string>


namespace TurboGE
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static Shader* Create(const std::string&, const std::string&);
	};
}