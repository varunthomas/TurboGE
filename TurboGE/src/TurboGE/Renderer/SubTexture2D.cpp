#include"tgepch.h"
#include"SubTexture2D.h"

namespace TurboGE
{

	std::unique_ptr<SubTexture2D> SubTexture2D::Create(std::shared_ptr<Texture2D>& texture, const glm::vec2& pos, const glm::vec2& dimension, const glm::vec2& spriteSize)
	{

		float maxHeight = texture->getHeight();
		float maxWidth = texture->getWidth();
		glm::vec2 maxDim = { maxWidth, maxHeight };

		glm::vec2 textCoord[] = {{ (pos.x * dimension.x) / maxWidth, (pos.y * dimension.y) / maxHeight },
		{ ((pos.x + spriteSize.x) * dimension.x) / maxWidth, (pos.y * dimension.y) / maxHeight },
		{ ((pos.x + spriteSize.x) * dimension.x) / maxWidth, ((pos.y + spriteSize.y) * dimension.y) / maxHeight },
		{ (pos.x * dimension.x) / maxWidth, ((pos.y + spriteSize.y) * dimension.y) / maxHeight }};

		return std::make_unique<SubTexture2D>(maxDim, textCoord, texture);
	}

}