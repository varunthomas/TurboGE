#pragma once
#include"tgepch.h"
#include"Core.h"
#include"Window.h"
#include"Imgui/ImguiLayer.h"
#include"Renderer/Shader.h"
#include"Renderer/Buffer.h"
#include"Renderer/VertexArray.h"
#include"Renderer/Renderer.h"
namespace TurboGE
{
	class TGE_API Application
	{
		std::unique_ptr<Window> m_window;
		//ImguiLayer* layerPtr;
		//std::unique_ptr<ImguiLayer> layer;
		//ImguiLayer layer;S
		ImguiLayer* layer;

		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_SquareVA;
		std::unique_ptr<VertexBuffer> m_SquareVB;
		std::shared_ptr<IndexBuffer> m_SquareIB;
		std::unique_ptr<Shader> m_SquareShader;

		std::unique_ptr<Renderer> m_Renderer;

		static Application* s_Instance;
	//protected:
		//inline void setLayerInstance(ImguiLayer* layer) { layerPtr = layer; }
		//inline ImguiLayer* getLayerInstance() { return layerPtr; }
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

