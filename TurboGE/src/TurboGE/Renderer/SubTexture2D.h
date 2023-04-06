#pragma once
#include<memory>
#include"TurboGE/Renderer/Texture.h"
#include<glm/glm.hpp>

namespace TurboGE
{
	class SubTexture2D
	{
		glm::vec2 maxDim;
		glm::vec2* textCoord;
		std::shared_ptr<Texture2D> m_Texture;
	public:
		SubTexture2D(glm::vec2 dim, glm::vec2* coord, std::shared_ptr<Texture2D> text)
			:maxDim{ dim }, m_Texture{text}
		{
			textCoord = new glm::vec2[4];
			textCoord[0] = coord[0];
			textCoord[1] = coord[1];
			textCoord[2] = coord[2];
			textCoord[3] = coord[3];
		};
		static std::unique_ptr<SubTexture2D> Create(std::shared_ptr<Texture2D>&, const glm::vec2&, const glm::vec2&, const glm::vec2&);

		inline glm::vec2* getCoordinates() { return textCoord; }
		inline std::shared_ptr<Texture2D> getTexture() { return m_Texture; }

		~SubTexture2D() { delete[] textCoord; };
	};
}