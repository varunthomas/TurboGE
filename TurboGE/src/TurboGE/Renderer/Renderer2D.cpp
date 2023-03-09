#include"tgepch.h"
#include"Renderer2D.h"
#include"glm/gtc/matrix_transform.hpp"

namespace TurboGE
{

	Renderer2D& Renderer2D::getInstance()
	{
		static Renderer2D m_Instance;
		return m_Instance;
	}

	void Renderer2D::Init()
	{

		m_SquareVA.reset(VertexArray::Create());
		m_SquareVB.reset(VertexBuffer::Create(maxVertices * sizeof(QuadVertices)));

		quadVerticesIndexBase = new QuadVertices[maxVertices];

		VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, AttribType::Float3, false }, {1, AttribType::Float4, false}, {2, AttribType::Float2, false} };
		layoutsq.MakeLayout();
		m_SquareVA->SetLayout(layoutsq);
		m_SquareVA->BindVertexBuffer();

		uint32_t* indicesSQ = new uint32_t[maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < maxIndices; i+=6)
		{
			indicesSQ[i + 0] = offset + 0;
			indicesSQ[i + 1] = offset + 1;
			indicesSQ[i + 2] = offset + 2;
			indicesSQ[i + 3] = offset + 2;
			indicesSQ[i + 4] = offset + 3;
			indicesSQ[i + 5] = offset + 0;

			offset += 4;
		}

		m_SquareIB.reset(IndexBuffer::Create(indicesSQ, maxIndices * sizeof(uint32_t)));
		m_SquareVA->setIndexBuffer(m_SquareIB);

		delete[] indicesSQ;

		m_WhiteTexture = Texture2D::Create();
		m_Shader.reset(Shader::Create("assets/shaders/Texture.glsl"));
		m_Shader->Bind();
		m_Shader->SetInt("u_Texture", 0);

		/*


		m_SquareVA.reset(VertexArray::Create());

		//EVERYTHING IS MADE OF TRIANGLE. SO WE NEED TO CREATE SQUARE USING 2 TRIANGLE. SO WE NEED TO SPECIFY 3+3 VERTEX POSITIONS. BUT 2 VERTICES
		//ARE DUPLICATES. THESE DUPLICATES WILL USE MEMORY. SO IN ORDER TO SAVE MEMORY WE REMOVE THE 2 DUPLICATES AND PROVIDE THE REMAINING 4 VERTICES
		//INDEX BUFFER WILL TAKE CARE OF CREATING SQUARE FROM 2 TRIANGLE WITHOUT USING DUPLICATES
		float sqVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		m_SquareVB.reset(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, AttribType::Float3, false }, {1, AttribType::Float2, false} };
		layoutsq.MakeLayout();
		m_SquareVA->SetLayout(layoutsq);
		m_SquareVA->BindVertexBuffer();
		//INDICES FOR SQUARE IS 0 1 2 FOR FIRST TRIANGLE. THEN 2 3 0 FOR SECOND TRIANGLE. INDICES ARE NUMBERED IN ANTICLOCKWISE DIR
		unsigned int indicesSQ[] = { 0, 1, 2, 2, 3, 0 };
		m_SquareIB.reset(IndexBuffer::Create(indicesSQ, sizeof(indicesSQ)));
		m_SquareVA->setIndexBuffer(m_SquareIB);


		//USE WHITE TEXTURE FOR USE WHEN SHADER IS USED FOR CREATING OBJECTS WITHOUT TEXTURE
		m_WhiteTexture = Texture2D::Create();
		m_Shader.reset(Shader::Create("assets/shaders/Texture.glsl"));
		m_Shader->Bind();
		m_Shader->SetInt("u_Texture", 0);
		*/
	}
	void Renderer2D::StartScene(const OrthographicCamera& camera)
	{
		quadIndexCount = 0;
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", camera.getViewProjectionMatrix());
		quadVerticesIndexPtr = quadVerticesIndexBase;
	}

	void Renderer2D::EndScene()
	{
		uint32_t size = (uint8_t*)quadVerticesIndexPtr - (uint8_t*)quadVerticesIndexBase;

		m_SquareVB->SetBatchData(size, quadVerticesIndexBase);
		
		m_SquareVA->DrawCommand(quadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		
		quadVerticesIndexPtr->position = {position.x, position.y, 0.0f};
		quadVerticesIndexPtr->color = color;
		quadVerticesIndexPtr->textCoord = { 0.0f, 0.0f };

		quadVerticesIndexPtr++;

		quadVerticesIndexPtr->position = { position.x + size.x, position.y, 0.0f };
		quadVerticesIndexPtr->color = color;
		quadVerticesIndexPtr->textCoord = { 1.0f, 0.0f };

		quadVerticesIndexPtr++;

		quadVerticesIndexPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		quadVerticesIndexPtr->color = color;
		quadVerticesIndexPtr->textCoord = { 1.0f, 1.0f };

		quadVerticesIndexPtr++;

		quadVerticesIndexPtr->position = { position.x, position.y + size.y, 0.0f };
		quadVerticesIndexPtr->color = color;
		quadVerticesIndexPtr->textCoord = { 0.0f, 1.0f };

		quadVerticesIndexPtr++;
		quadIndexCount += 6;


	}

	/*void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		m_Shader->Bind();

		m_Shader->SetFloat4("u_Color", color);
		m_Shader->SetMat4("u_Transform", transform);

		m_WhiteTexture->Bind();

		m_SquareVA->Bind();
		m_SquareVA->DrawCommand();

	}*/

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, std::unique_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, std::unique_ptr<Texture2D>& texture)
	{
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		m_Shader->Bind();
		m_Shader->SetFloat4("u_Color", glm::vec4(1.0f));
		m_Shader->SetMat4("u_Transform", transform);

		texture->Bind();

		m_SquareVA->Bind();
		m_SquareVA->DrawCommand();

	}

}