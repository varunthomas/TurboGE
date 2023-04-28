#pragma once
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Texture.h"
#include"TurboGE/Renderer/SubTexture2D.h"

namespace TurboGE
{

	class Renderer2D
	{
		Renderer2D() = default;
		~Renderer2D() = default;
		std::unique_ptr<VertexArray> m_SquareVA;
		std::unique_ptr<Shader> m_SquareShader;
		std::shared_ptr<Texture2D> m_WhiteTexture;
		std::shared_ptr<VertexBuffer> m_SquareVB;
		std::shared_ptr<IndexBuffer> m_SquareIB;

		std::unique_ptr<VertexArray> m_CircleVA;
		std::shared_ptr<VertexBuffer> m_CircleVB;
		std::unique_ptr<Shader> m_CircleShader;
		std::shared_ptr<IndexBuffer> m_CircleIB;

		const uint32_t maxQuads = 20000;
		const uint32_t maxIndices = maxQuads * 6;
		const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxTextures = 2;

		struct QuadVertices
		{
			glm::vec3 position;
			glm::vec4 color;
			glm::vec2 textCoord;
			float textIndex;
			float tilingFactor;
			int entityID;
		};

		struct CircleVertices
		{
			glm::vec3 worldPosition;
			glm::vec3 localPosition;
			glm::vec4 color;
			float thickness;
			float fade;
			int entityID;
		};

		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};

		Statistics stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData cameraData;
		std::unique_ptr<UniformBuffer> cameraUB;

		std::vector<QuadVertices> quadVerticesIndexBase;
		std::vector<CircleVertices> circleVerticesIndexBase;
		std::array<std::shared_ptr<Texture2D>, maxTextures> textures;

		uint32_t quadIndexCount = 0;
		uint32_t circleIndexCount = 0;
		uint32_t m_IndexSquare = 0;
		uint32_t m_IndexCircle = 0;
		uint32_t textureSlot = 1;

		std::array<glm::vec4, 4> quadVertexPos;

	public:
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;
		static Renderer2D& getInstance();

		
		void Init();
		void StartScene(EditorCamera&);
		void StartScene(const Camera&);
		void StartScene(const GameCamera&, const glm::mat4&);
		void EndScene();
		void ShutDown();
		void ResetCounters();


		void ResetStats();
		Statistics GetStats();

		void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		
		template<class Position>
		void DrawQuad(const Position& position, const glm::vec4& color, const int entityID = -1, const glm::vec2& size = {1.0f, 1.0f})
		{
			if constexpr (std::is_same_v<Position, glm::vec2>)
			{
				DrawQuad(glm::vec3(position.x, position.y, 0.0f), color, entityID, size);
			}
			else
			{
				if (quadIndexCount >= maxIndices)
				{
					EndScene();
					ResetCounters();
				}
				const float textSlot = 0.0f;
				const float tilingFactor = 1.0f;
				constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

				for (size_t i{}; i < 4; i++)
				{
					if constexpr (std::is_same_v<Position, glm::mat4>)
					{
						quadVerticesIndexBase.at(m_IndexSquare) = { position * quadVertexPos[i], color, textureCoords[i], textSlot, tilingFactor, entityID };
					}
					else
					{
						quadVerticesIndexBase.at(m_IndexSquare) = { position, color, textureCoords[i], textSlot, tilingFactor};
					}
					m_IndexSquare++;
				}
				quadIndexCount += 6;
				stats.quadCount++;
			}
		}

		template<class Position, class TexType>
		void DrawQuad(const Position& position, const glm::vec2& size, TexType& textureClass, float tilingFactor, const int entityID = -1)
		{
			if constexpr (std::is_same_v < TexType, std::unique_ptr<SubTexture2D>>)
			{
				if (quadIndexCount >= maxIndices || textureSlot >= maxTextures)
				{
					EndScene();
					ResetCounters();
				}

				constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

				glm::vec2* textCoord = textureClass->getCoordinates();

				std::shared_ptr<Texture2D> texture = textureClass->getTexture();

				if (textures[textureSlot].get() == nullptr || *textures[textureSlot].get() != *texture.get())
				{
					textures[textureSlot] = texture;
				}


				quadVerticesIndexBase[m_IndexSquare] = { position, color, textCoord[0], (float)textureSlot, tilingFactor, entityID };
				m_IndexSquare++;

				quadVerticesIndexBase[m_IndexSquare] = { { position.x + size.x, position.y, 0.0f }, color, textCoord[1], (float)textureSlot, tilingFactor, entityID };
				m_IndexSquare++;

				quadVerticesIndexBase[m_IndexSquare] = { { position.x + size.x, position.y + size.y, 0.0f }, color, textCoord[2], (float)textureSlot, tilingFactor, entityID };
				m_IndexSquare++;

				quadVerticesIndexBase[m_IndexSquare] = { { position.x, position.y + size.y, 0.0f }, color, textCoord[3], (float)textureSlot, tilingFactor, entityID };
				m_IndexSquare++;


				textureSlot++;
				quadIndexCount += 6;

				stats.quadCount++;
			}
			else
			{
				if constexpr (std::is_same_v < Position, glm::vec3>)
				{
					DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, textureClass, tilingFactor, entityID);
				}
				else
				{
					if (quadIndexCount >= maxIndices || textureSlot >= maxTextures)
					{
						EndScene();
						ResetCounters();
					}

					constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
					constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
					if (textures[textureSlot].get() == nullptr || *textures[textureSlot].get() != *textureClass.get())
					{
						textures[textureSlot] = textureClass;
					}

					for (size_t i{}; i < 4; i++)
					{
						if constexpr (std::is_same_v<Position, glm::mat4>)
						{
							quadVerticesIndexBase.at(m_IndexSquare) = { position * quadVertexPos[i], color, textureCoords[i], (float)textureSlot, tilingFactor, entityID };
						}
						else
						{
							quadVerticesIndexBase.at(m_IndexSquare) = { position, color, textureCoords[i], (float)textureSlot, tilingFactor, entityID };
						}
						m_IndexSquare++;
					}

					textureSlot++;
					quadIndexCount += 6;

					stats.quadCount++;
				}
			}
		}
	};
}