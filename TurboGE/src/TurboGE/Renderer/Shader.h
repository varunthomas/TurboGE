#include<string>

namespace TurboGE
{
	class Shader
	{
		uint32_t m_RendererID;
	public:
		Shader(std::string&, std::string&);
		~Shader();
		void Bind();
		void Unbind();
	};
}