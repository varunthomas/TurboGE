#include"tgepch.h"
#include"Window.h"
#include "Application.h"
#include "Test.h"
#include "Imgui/ImguiLayer.h"
#include"imgui_tables.cpp"
#include"GLFW/glfw3.h"
#include"Time.h"
//#include"Renderer/Camera.h"
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
		s = new Example(); //TEMP

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
		float curr_time = 0.0f, prev_time = 0.0f;
		while (true)
		{
			curr_time = (float)glfwGetTime();
			Time deltaTime = curr_time - prev_time;
			prev_time = curr_time;
			TURBO_CORE_ERR("Time is {0}", deltaTime);
			s->onUpdate(deltaTime);
			layer->onUpdate();
			m_window->onUpdate();
		}
	}

}
