#pragma once
#include"TurboGE/Window.h"

#include "TurboGE/Logger.h"

namespace TurboGE
{
	
	class WindowsWindow : public Window
	{

		GLFWwindow* m_window;
		bool m_Glfw_init = false;
	public:
		WindowsWindow(WindowProp props)
		{
			Init(props);
		}
		WindowsWindow(const WindowsWindow& ref)
		{
			TURBO_CORE_TRC("Copy cons called\n");
		}

		void Init(WindowProp) override;
		void shutDown() override;
		void onUpdate() override;
		void SetVSync(bool) override;

		
		~WindowsWindow();
	};
}

