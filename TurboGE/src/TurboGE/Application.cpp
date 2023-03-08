#include"tgepch.h"
#include"Window.h"
#include "Application.h"
#include "Test.h"
#include "Imgui/ImguiLayer.h"
#include"GLFW/glfw3.h"
#include"Time.h"

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
		//s = new Example(); //TEMP
		s = new Sandbox2D(); //TEMP

	}

	void Application::OnEvent(Event& e)
	{
		layer->onEvent(e);
		if (e.getEventType() == EventType::WindowSizeEvent)
		{
			//STOP RENDERING IF MINIMIZED
			if (dynamic_cast<WindowSizeEvent&>(e).GetHeight() == 0)
			{
				m_minimized = true;
				
			}
			else
			{
				m_minimized = false;
			}
		}
		else if (e.getEventType() == EventType::WindowCloseEvent)
		{
			m_Running = false;
		}
		s->onEvent(e);
	}
	Application::~Application()
	{

	}
	void Application::Run()
	{
		float curr_time = 0.0f, prev_time = 0.0f;
		while (m_Running)
		{
			curr_time = (float)glfwGetTime();
			Time deltaTime = curr_time - prev_time;
			prev_time = curr_time;
			layer->Begin();
			if (!m_minimized)
			{
				s->onUpdate(deltaTime);
				s->renderCustom();
			}
			layer->End();
			m_window->onUpdate();
		}
	}

}
