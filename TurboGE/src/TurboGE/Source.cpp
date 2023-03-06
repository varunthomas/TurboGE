#include"tgepch.h"
#include"Source.h"
#include"imgui.h"
#include"glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include"Platform/OpenGL/OpenGLShader.h"
#include"Events/AppEvent.h"

	Example::Example()
	{
		m_Renderer.reset(TurboGE::Renderer::Create());
		m_Renderer->Init();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f,0.2f,0.8f,1.0f,
			0.5f, -0.5f, 0.0f, 0.2f,0.3f,0.8f,1.0f,
			0.0f, 0.5f, 0.0f, 0.8f,0.8f,0.2f,1.0f
		};

		m_VertexArray.reset(TurboGE::VertexArray::Create());
		m_VertexBuffer.reset(TurboGE::VertexBuffer::Create(vertices, sizeof(vertices)));
		TurboGE::VertexLayout layout;
		layout.m_attribVec = { {0, TurboGE::AttribType::Float3, false },{1, TurboGE::AttribType::Float4, false} };
		layout.MakeLayout();
		m_VertexArray->SetLayout(layout);
		m_VertexArray->BindVertexBuffer();
		unsigned int indices[] = { 0,1,2 };
		m_IndexBuffer.reset(TurboGE::IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->setIndexBuffer(m_IndexBuffer);


		m_SquareVA.reset(TurboGE::VertexArray::Create());

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
		m_SquareVB.reset(TurboGE::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		TurboGE::VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, TurboGE::AttribType::Float3, false }, {1, TurboGE::AttribType::Float2, false} };
		layoutsq.MakeLayout();
		m_SquareVA->SetLayout(layoutsq);
		m_SquareVA->BindVertexBuffer();
		//INDICES FOR SQUARE IS 0 1 2 FOR FIRST TRIANGLE. THEN 2 3 0 FOR SECOND TRIANGLE. INDICES ARE NUMBERED IN ANTICLOCKWISE DIR
		unsigned int indicesSQ[] = { 0, 1, 2, 2, 3, 0 };
		m_SquareIB.reset(TurboGE::IndexBuffer::Create(indicesSQ, sizeof(indicesSQ)));
		m_SquareVA->setIndexBuffer(m_SquareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
			)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
			)";

		//m_Shader.reset(TurboGE::Shader::Create(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_SquareShader.reset(TurboGE::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));

		m_TextureShader.reset(TurboGE::Shader::Create("assets/shaders/Texture.glsl"));


		m_TextureShader->Bind();
		dynamic_cast<TurboGE::OpenGLShader*>(m_TextureShader.get())->UploadUniformInt("u_Texture", 0);

		m_Texture = TurboGE::Texture2D::Create("assets/textures/pikachu.png");

		m_CheckTexture = TurboGE::Texture2D::Create("assets/textures/Checkerboard.png");
	}

	void Example::onUpdate(TurboGE::Time delta)
	{
		m_Renderer->setClearColor();
		m_Renderer->Clear();

		m_CameraController.OnUpdate(delta);

		m_Renderer->StartScene(m_CameraController.GetCamera());

		m_SquareShader->Bind();
		dynamic_cast<TurboGE::OpenGLShader*>(m_SquareShader.get())->uploadUniformFloat3("u_Color", m_SquareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 20;x++)
		{
			for (int y = 0;y < 20;y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				m_Renderer->Submit(m_SquareShader, m_SquareVA, transform);
			}
		}

		
		m_CheckTexture->Bind();
		m_Renderer->Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Texture->Bind();
		m_Renderer->Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	}

	void Example::onEvent(TurboGE::Event& e)
	{
		//RENDER NEW AREAS AFTER SIZE IS INCREASED
		if (e.getEventType() == TurboGE::EventType::WindowSizeEvent)
		{
			auto& winEvent = dynamic_cast<TurboGE::WindowSizeEvent&>(e);
			m_Renderer->setViewPort(winEvent.GetWidth(), winEvent.GetHeight());
		}

		m_CameraController.onEvent(e);
	}

	void Example::renderCustom()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
