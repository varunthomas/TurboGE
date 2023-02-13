#pragma once
#include"tgepch.h"
#include"Core.h"
#include"Window.h"
#include"Imgui/ImguiLayer.h"
namespace TurboGE
{
	class TGE_API Application
	{
		std::unique_ptr<Window> m_window;
		ImguiLayer* layerPtr;
		//std::unique_ptr<ImguiLayer> layer;
		//ImguiLayer layer;S
		//ImguiLayer* layer = new ImguiLayer();
		static Application* s_Instance;
	protected:
		inline void setLayerInstance(ImguiLayer* layer) { layerPtr = layer; }
		inline ImguiLayer* getLayerInstance() { return layerPtr; }
	public:
		Application();
		virtual ~Application();
		inline Window& GetWindow() { return *m_window; }

		inline static Application& Get() { return *s_Instance; }
		void Run();
		void OnEvent(Event& e);
	};
	Application* CreateApplication();

}

