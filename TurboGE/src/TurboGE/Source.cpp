#include"tgepch.h"
#include"Source.h"
#include"GLFW/glfw3.h"
#include"imgui.h"
#include"glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include"Platform/OpenGL/OpenGLShader.h"

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



		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(TurboGE::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = TurboGE::Texture2D::Create("assets/textures/pikachu.png");

		m_TextureShader->Bind();
		dynamic_cast<TurboGE::OpenGLShader*>(m_TextureShader.get())->UploadUniformInt("u_Texture", 0);
	}

	void Example::onUpdate(Time delta)
	{
		m_Renderer->setClearColor();
		m_Renderer->Clear();

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

		m_Texture->Bind();
		m_Renderer->Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//m_SquareVA->Bind();
		//glDrawElements(GL_TRIANGLES, m_SquareIB->getCount(), GL_UNSIGNED_INT, nullptr);

		//m_Shader->Bind();
		//m_Renderer->Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));
		//m_VertexArray->Bind();
		//glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
	}


	void Example::renderCustom()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
