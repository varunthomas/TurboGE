#pragma once
#include"TurboGE/Renderer/Texture.h"

namespace TurboGE
{
	class OpenGLTexture2D : public Texture2D
	{
		std::string m_path;
		uint32_t m_height;
		uint32_t m_width;
		uint32_t m_rendererID;
	public:
		OpenGLTexture2D(const std::string& path);

		void Bind(uint32_t slot = 0) override;
		void Unbind() override;
		~OpenGLTexture2D();
	};
}