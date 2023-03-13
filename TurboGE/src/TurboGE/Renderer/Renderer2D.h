#pragma once
#include<array>
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Texture.h"
#include"TurboGE/Renderer/SubTexture2D.h"

//WE ARE CREATING STATIC CLASS BECAUSE RENDERERS WILL ONLY HAVE 1 INSTANCE. WE CAN USE OBJECT TO INSTANTIATE RENDERER CLASS BUT WE DONT NEED
//DIFFERENT INSTANCES
namespace TurboGE
{

	class Renderer2D
	{
		Renderer2D() = default;
		~Renderer2D() = default;
		std::unique_ptr<VertexArray> m_SquareVA;
		std::unique_ptr<Shader> m_Shader;
		std::shared_ptr<Texture2D> m_WhiteTexture;
		std::unique_ptr<VertexBuffer> m_SquareVB;
		std::shared_ptr<IndexBuffer> m_SquareIB;

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
		};

		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
		};

		Statistics stats;

		std::vector<QuadVertices> quadVerticesIndexBase;
		std::array<std::shared_ptr<Texture2D>, maxTextures> textures;

		uint32_t quadIndexCount = 0;
		uint32_t m_Index = 0;
		uint32_t textureSlot = 1;

	public:
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;
		static Renderer2D& getInstance();

		
		void Init();
		void StartScene(const OrthographicCamera&);
		void EndScene();
		void ShutDown();
		void ResetCounters();


		void ResetStats();
		Statistics GetStats();

		void DrawQuad(const glm::vec2&, const glm::vec2&, const glm::vec4&);
		void DrawQuad(const glm::vec3&, const glm::vec2&, const glm::vec4&);

		void DrawQuad(const glm::vec2&, const glm::vec2&, std::shared_ptr<Texture2D>&, float);
		void DrawQuad(const glm::vec3&, const glm::vec2&, std::shared_ptr<Texture2D>&, float);

		void DrawQuad(const glm::vec3&, const glm::vec2&, std::unique_ptr<SubTexture2D>&, float);
	};
}