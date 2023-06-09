#pragma once
#include"tgepch.h"
#include"Core.h"
#include"Window.h"
#include"Imgui/ImguiLayer.h"
#include"Events/Event.h"
#include"../../Editor/src/Editor2D.h"  //TEMP



namespace TurboGE
{
	class Application
	{
		bool m_minimized = false;
		std::unique_ptr<Window> m_window;
		ImguiLayer* layer;
		
		Editor2D* s; //TEMP
		bool m_Running = true;


		static Application* s_Instance;
	public:
		Application();
		virtual ~Application();
		inline Window& GetWindow() { return *m_window; }

		inline static Application& Get() { return *s_Instance; }
		void Run();
		void OnEvent(Event& e);
		void Close();
		void Minimize();
		void RestoreUp();
		void RestoreDown();
	};
	Application* CreateApplication();

}

