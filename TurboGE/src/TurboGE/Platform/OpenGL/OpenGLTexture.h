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
		OpenGLTexture2D();
		bool operator!=(const Texture2D& tex) override { return m_rendererID != ((OpenGLTexture2D&)tex).m_rendererID; }

		void Bind(uint32_t slot = 0) override;
		void Unbind() override;

		uint32_t getHeight() override { return m_height; };
		uint32_t getWidth() override { return m_width; };

		~OpenGLTexture2D();
	};
}