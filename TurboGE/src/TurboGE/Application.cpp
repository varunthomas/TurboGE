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
		//layer->onAttach();
		TURBO_CORE_ERR("Setting callback");
		m_window->setCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	void Application::OnEvent(Event& e)
	{
		getLayerInstance()->onEvent(e);
		//TURBO_CORE_ERR("Callback done {0}", e.getString());
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		
		while (true)
		{
			//glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			getLayerInstance()->onUpdate();
			m_window->onUpdate();
		}
	}

}
