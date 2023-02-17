#include"tgepch.h"
#include"Window.h"
#include "Application.h"
#include "Test.h"
#include "Imgui/ImguiLayer.h"
#include"imgui_tables.cpp"
#include<glad/glad.h>

namespace TurboGE
{
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		s_Instance = this;
		m_window = Window::Create();
		layer = new ImguiLayer() ;
		layer->onAttach();
		TURBO_CORE_ERR("Setting callback");
		m_window->setCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Renderer.reset(Renderer::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f,0.2f,0.8f,1.0f,
			0.5f, -0.5f, 0.0f, 0.2f,0.3f,0.8f,1.0f,
			0.0f, 0.5f, 0.0f, 0.8f,0.8f,0.2f,1.0f
		};

		m_VertexArray.reset(VertexArray::Create());
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		VertexLayout layout;
		layout.m_attribVec = { {0, AttribType::Float3, false },{1, AttribType::Float4, false} };
		layout.MakeLayout();
		m_VertexArray->SetLayout(layout);
		m_VertexArray->BindVertexBuffer();
		unsigned int indices[] = {0,1,2};
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->setIndexBuffer(m_IndexBuffer);


		m_SquareVA.reset(VertexArray::Create());

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
		m_SquareVB.reset(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		VertexLayout layoutsq;
		layoutsq.m_attribVec = { {0, AttribType::Float3, false } };
		layoutsq.MakeLayout();
		m_SquareVA->SetLayout(layoutsq);
		m_SquareVA->BindVertexBuffer();
		//INDICES FOR SQUARE IS 0 1 2 FOR FIRST TRIANGLE. THEN 2 3 0 FOR SECOND TRIANGLE. INDICES ARE NUMBERED IN ANTICLOCKWISE DIR
		unsigned int indicesSQ[] = { 0, 1, 2, 2, 3, 0 };
		m_SquareIB.reset(IndexBuffer::Create(indicesSQ, sizeof(indicesSQ)));
		m_SquareVA->setIndexBuffer(m_SquareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}
			)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
			)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_SquareShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void Application::OnEvent(Event& e)
	{
		layer->onEvent(e);
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		
		while (true)
		{
			m_Renderer->setClearColor();
			m_Renderer->Clear();
			//glClearColor(0.5f, 0.5f, 0.5f, 1);
			//glClear(GL_COLOR_BUFFER_BIT);

			m_SquareShader->Bind();

			m_Renderer->Submit(m_SquareVA);
			//m_SquareVA->Bind();
			//glDrawElements(GL_TRIANGLES, m_SquareIB->getCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_Renderer->Submit(m_VertexArray);
			//m_VertexArray->Bind();
			//glDrawElements(GL_TRIANGLES, m_IndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
			layer->onUpdate();
			m_window->onUpdate();
		}
	}

}
