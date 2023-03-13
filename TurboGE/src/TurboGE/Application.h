#pragma once
#include"tgepch.h"
#include"Core.h"
#include"Window.h"
#include"Imgui/ImguiLayer.h"
#include"Events/Event.h"
//#include"Source.h" //TEMP
//#include"../../Sandbox/src/Sandbox2D.h" //TEMP
#include"../../Editor/src/Editor2D.h"  //TEMP



namespace TurboGE
{
	class TGE_API Application
	{
		bool m_minimized = false;
		std::unique_ptr<Window> m_window;
		ImguiLayer* layer;
		
		//Sandbox2D* s; //TEMP
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
	};
	Application* CreateApplication();

}

