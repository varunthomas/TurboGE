#pragma once
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Texture.h"

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
		std::unique_ptr<Texture2D> m_WhiteTexture;
		std::unique_ptr<VertexBuffer> m_SquareVB;
		std::shared_ptr<IndexBuffer> m_SquareIB;

		const uint32_t maxQuads = 10000;
		const uint32_t maxIndices = maxQuads * 6;
		const uint32_t maxVertices = maxQuads * 4;

		struct QuadVertices
		{
			glm::vec3 position;
			glm::vec4 color;
			glm::vec2 textCoord;
		};

		QuadVertices* quadVerticesIndexPtr = nullptr;
		QuadVertices* quadVerticesIndexBase = nullptr;
		uint32_t quadIndexCount = 0;
		
	public:
		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;
		static Renderer2D& getInstance();

		
		void Init();
		void StartScene(const OrthographicCamera&);
		void EndScene();
		void ShutDown();

		void DrawQuad(const glm::vec2&, const glm::vec2&, const glm::vec4&);
		void DrawQuad(const glm::vec3&, const glm::vec2&, const glm::vec4&);
		void DrawQuad(const glm::vec2&, const glm::vec2&, std::unique_ptr<Texture2D>&);
		void DrawQuad(const glm::vec3&, const glm::vec2&, std::unique_ptr<Texture2D>&);
	};
}