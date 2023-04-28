#include"tgepch.h"
#include"Renderer2D.h"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtx/string_cast.hpp"
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

		VertexLayout layoutsq;		//LAYOUT, DATATYPE, NORMALIZED, ENTITYID
		layoutsq.m_attribVec = {	{0, AttribType::Float3, false, false	},
									{1, AttribType::Float4, false, false	},
									{2, AttribType::Float2, false, false	},
									{3, AttribType::Float,	false, false	},
									{4, AttribType::Float,  false, false	},
									{5, AttribType::Int,    false, true		}
								};
		layoutsq.MakeLayout();
		m_SquareVA->SetLayout(layoutsq);
		m_SquareVA->BindVertexBuffer(m_SquareVB);

		//CIRCLE
		
		m_CircleVA = VertexArray::Create();
		m_CircleVB = VertexBuffer::Create(maxVertices * sizeof(CircleVertices));

		circleVerticesIndexBase.resize(maxIndices);

		VertexLayout layoutCircle;		//LAYOUT, DATATYPE, NORMALIZED, ENTITYID
		layoutCircle.m_attribVec = {	{0, AttribType::Float3, false, false	},
										{1, AttribType::Float3, false, false	},
										{2, AttribType::Float4, false, false	},
										{3, AttribType::Float,	false, false	},
										{4, AttribType::Float,  false, false	},
										{5, AttribType::Int,    false, true		}
		};
		layoutCircle.MakeLayout();
		m_CircleVA->SetLayout(layoutCircle);
		m_CircleVA->BindVertexBuffer(m_CircleVB);

		

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

		m_CircleVA->setIndexBuffer(m_SquareIB); //Intentional


		m_WhiteTexture = Texture2D::Create();
		m_SquareShader = Shader::Create("assets/shaders/Texture.glsl");
		m_CircleShader = Shader::Create("assets/shaders/Circle.glsl");

		cameraUB = UniformBuffer::Create(sizeof(CameraData), 0);

		textures[0] = m_WhiteTexture;

	}

	void Renderer2D::StartScene(EditorCamera& camera)
	{
		textureSlot = 1;
		quadIndexCount = 0;
		circleIndexCount = 0;
		m_IndexSquare = 0;
		m_IndexCircle = 0;
		cameraData.ViewProjection = camera.GetViewProjection();
		cameraUB->SetData(&cameraData, sizeof(CameraData));
	}
	void Renderer2D::StartScene(const Camera& camera)
	{
		textureSlot = 1;
		quadIndexCount = 0;
		circleIndexCount = 0;
		m_IndexSquare = 0;
		m_IndexCircle = 0;
		cameraData.ViewProjection = camera.getViewProjectionMatrix();
		cameraUB->SetData(&cameraData, sizeof(CameraData));
	}

	void Renderer2D::StartScene(const GameCamera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewproj = camera.GetProjection() * glm::inverse(transform);
		textureSlot = 1;
		quadIndexCount = 0;
		circleIndexCount = 0;
		m_IndexSquare = 0;
		m_IndexCircle = 0;
		cameraData.ViewProjection = viewproj;
		cameraUB->SetData(&cameraData, sizeof(CameraData));
	}

	void Renderer2D::EndScene()
	{
		if (quadIndexCount)
		{
			uint32_t size = m_IndexSquare * sizeof(QuadVertices);
			m_SquareVB->SetBatchData(size, &quadVerticesIndexBase[0]);

			for (int i = 0; i < textureSlot; i++)
			{
				textures[i]->Bind(i);
			}
			m_SquareShader->Bind();
			m_SquareVA->Bind();
			m_SquareVA->DrawCommand(quadIndexCount);
			stats.drawCalls++;
		}

		if (circleIndexCount)
		{
			uint32_t size = m_IndexCircle * sizeof(CircleVertices);
			m_CircleVB->SetBatchData(size, &circleVerticesIndexBase[0]);

			m_CircleShader->Bind();
			m_CircleVA->Bind();
			m_CircleVA->DrawCommand(circleIndexCount);
			stats.drawCalls++;
		}
		
	}

	void Renderer2D::ResetCounters()
	{
		textureSlot = 1;
		quadIndexCount = 0;
		circleIndexCount = 0;
		m_IndexSquare = 0;
		m_IndexCircle = 0;
	}
	

	void Renderer2D::ResetStats()
	{
		memset(&stats, 0, sizeof(Statistics));
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
	{

		for (size_t i{}; i < 4; i++)
		{
			circleVerticesIndexBase.at(m_IndexCircle) = { transform * quadVertexPos[i],  quadVertexPos[i]*2.0f, color, thickness, fade, entityID };
			m_IndexCircle++;
		}
		circleIndexCount += 6;
		stats.quadCount++;

	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return stats;
	}
}