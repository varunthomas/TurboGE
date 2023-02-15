#include"tgepch.h"
#include"Window.h"
#include "Application.h"
#include "Test.h"
#include "Imgui/ImguiLayer.h"
#include"imgui_tables.cpp"
#include<glad/glad.h>
//#include<glad/glad.h>

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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[] = { 0,1,2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			
			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
			}
			)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(0.8,0.2,0.2,1.0);
			}
			)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	void Application::OnEvent(Event& e)
	{
		layer->onEvent(e);
		//TURBO_CORE_ERR("Callback done {0}", e.getString());
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		
		while (true)
		{
			glClearColor(0.5f, 0.5f, 0.5f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			layer->onUpdate();
			m_window->onUpdate();
		}
	}

}
