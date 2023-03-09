#include"tgepch.h"
#include"OpenGLTexture.h"
#include"stb_image.h"
#include<glad/glad.h>

namespace TurboGE
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_path{ path }
	{
		int height, width, channels;
		GLenum inputFormat = 0, outputFormat = 0;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			std::cout << "Data is null "  << path << std::endl;
		}

		if (channels == 4)
		{
			inputFormat = GL_RGBA8;
			outputFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			inputFormat = GL_RGB8;
			outputFormat = GL_RGB;
		}
		m_height = height;
		m_width = width;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, inputFormat, m_width, m_height);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//BELOW CALLS ARE PRESENT IN CHERNO
		//glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, outputFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D()
	{
		uint32_t blankTexture = 0xffffffff;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, GL_RGBA8, 1, 1);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//BELOW CALLS ARE PRESENT IN CHERNO
		//glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_rendererID, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &blankTexture);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_rendererID);
	}

	void OpenGLTexture2D::Unbind()
	{

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererID);
	}
}