#include"tgepch.h"
#include"Source.h"
#include"GLFW/glfw3.h"

	Example::Example()
	{
		m_Input.reset(new TurboGE::Input());
		m_Renderer.reset(TurboGE::Renderer::Create());

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
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		m_SquareVB.reset(TurboGE::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		TurboGE::VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, TurboGE::AttribType::Float3, false } };
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

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
		m_Shader.reset(new TurboGE::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;		

			void main()
			{
				
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";
		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(new TurboGE::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void Example::onUpdate(Time delta)
	{
		m_Renderer->setClearColor();
		m_Renderer->Clear();
		//glClearColor(0.5f, 0.5f, 0.5f, 1);
		//glClear(GL_COLOR_BUFFER_BIT);

		if (m_Input->isKeyPressed(GLFW_KEY_UP))
		{
			m_CameraPos.y += 5.0f * delta;
		}
		else if (m_Input->isKeyPressed(GLFW_KEY_DOWN))
		{
			m_CameraPos.y -= 5.0f * delta;
		}
		else if (m_Input->isKeyPressed(GLFW_KEY_LEFT))
		{
			m_CameraPos.x -= 5.0f * delta;
		}
		else if (m_Input->isKeyPressed(GLFW_KEY_RIGHT))
		{
			m_CameraPos.x += 5.0f * delta;
		}
		else if (m_Input->isKeyPressed(GLFW_KEY_A))
		{
			m_CameraRot -= 4.0f * delta;
		}
		else if (m_Input->isKeyPressed(GLFW_KEY_D))
		{
			m_CameraRot += 4.0f * delta;
		}
		m_Camera.setPosition(m_CameraPos);
		m_Camera.setRotation(m_CameraRot);

		m_Renderer->StartScene(m_Camera);
		//m_SquareShader->Bind();

		m_Renderer->Submit(m_SquareShader, m_SquareVA);
		//m_SquareVA->Bind();
		//glDrawElements(GL_TRIANGLES, m_SquareIB->getCount(), GL_UNSIGNED_INT, nullptr);

		//m_Shader->Bind();
		m_Renderer->Submit(m_Shader, m_VertexArray);
		//m_VertexArray->Bind();
		//glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
	}
