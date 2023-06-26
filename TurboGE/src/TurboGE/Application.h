#pragma once
#include"tgepch.h"
#include"Core.h"
#include"Window.h"
#include"Imgui/ImguiLayer.h"
#include"Events/Event.h"
#include"Layer.h"

namespace TurboGE
{
	class Application
	{
		bool m_minimized = false;
		std::unique_ptr<Window> m_window;
		ImguiLayer* layer;

		bool m_Running = true;
		Layer* m_Layer{};


		static Application* s_Instance;
	public:
		Application();
		virtual ~Application();

		void GetLayer(Layer*);

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

