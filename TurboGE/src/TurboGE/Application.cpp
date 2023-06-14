#include"tgepch.h"
#include"Window.h"
#include "Application.h"
#include "Imgui/ImguiLayer.h"
#include"GLFW/glfw3.h"
#include"Time.h"
#include"Scripting/Scripting.h"
#include<filesystem>



namespace TurboGE
{
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{

		PyScript::Init();
		s_Instance = this;
		m_window = Window::Create();
		layer = new ImguiLayer() ;
		layer->onAttach();
		m_window->setCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		s = new Editor2D();

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
			Close();
		}
		s->onEvent(e);
	}
	Application::~Application()
	{
		PyScript::ShutDown();
		delete s;
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

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Minimize()
	{
		glfwIconifyWindow((GLFWwindow*)GetWindow().GetNativeWindow());
	}

	void Application::RestoreDown()
	{
		glfwSetWindowSize((GLFWwindow*)GetWindow().GetNativeWindow(), 1280, 720);
	}
	void Application::RestoreUp()
	{
		glfwSetWindowSize((GLFWwindow*)GetWindow().GetNativeWindow(), 1920, 1080);
	}
}
