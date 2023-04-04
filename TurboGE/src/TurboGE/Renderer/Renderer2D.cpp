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

		quadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		quadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		quadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		quadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		m_SquareVA = VertexArray::Create();
		m_SquareVB = VertexBuffer::Create(maxVertices * sizeof(QuadVertices));
		
		quadVerticesIndexBase.resize(maxIndices);

		VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, AttribType::Float3, false }, {1, AttribType::Float4, false}, {2, AttribType::Float2, false}, {3, AttribType::Float, false },  {4, AttribType::Float, false } };
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

		m_SquareIB = IndexBuffer::Create(indicesSQ, maxIndices * sizeof(uint32_t));
		m_SquareVA->setIndexBuffer(m_SquareIB);

		delete[] indicesSQ;

		m_WhiteTexture = Texture2D::Create();
		m_Shader = Shader::Create("assets/shaders/Texture.glsl");
		
		//m_Shader->Bind();
		//m_Shader->SetInt("u_Texture", 0);

		int slots[maxTextures];

		for (uint32_t i = 0; i < maxTextures; i++)
		{
			slots[i] = i;
		}
		m_Shader->Bind();
		m_Shader->SetIntArray("u_Texture", slots, maxTextures);

		textures[0] = m_WhiteTexture;

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
		textureSlot = 1;
		quadIndexCount = 0;
		m_Index = 0;
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		uint32_t size = m_Index * sizeof(QuadVertices);
		m_SquareVB->SetBatchData(size, &quadVerticesIndexBase[0]);
		
		for (uint32_t i = 0; i < textureSlot; i++)
		{
			textures[i]->Bind(i);
		}
		m_SquareVA->DrawCommand(quadIndexCount);
		stats.drawCalls++;
		
	}

	void Renderer2D::ResetCounters()
	{
		textureSlot = 1;
		quadIndexCount = 0;
		m_Index = 0;
	}
	

	void Renderer2D::ResetStats()
	{
		memset(&stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return stats;
	}

	Renderer2D::~Renderer2D()
	{
		std::cout << "Deleted renderer2D\n";
	}
}