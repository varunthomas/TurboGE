#pragma once
#include"tgepch.h"
#include"Core.h"
#include<GLFW/glfw3.h>
#include"Logger.h"
#include"Events/Event.h"
namespace TurboGE
{
	struct WindowProp
	{
		std::string title;
		unsigned int height;
		unsigned int width;

		WindowProp(const std::string& title = "TurboGE", unsigned int height = 720, unsigned int width = 1280)
			:title{ title }, height{ height }, width{ width }
		{
		}
	};

	class TGE_API Window
	{
	protected:
		std::string m_title;

	public:
		Window()
		{

		}
		Window(const Window& ref)
		{
			TURBO_CORE_TRC("Copy cons called\n");
		}
		using CallBackFn = std::function<void(Event&)>;
		static std::unique_ptr<Window> Create(WindowProp props = WindowProp());
		//static Window* Create(WindowProp props = WindowProp());
		inline void setCallback(CallBackFn fn) { m_data.callbackFn = fn; }
		inline unsigned int GetWidth() const {  return m_data.width; }
		inline unsigned int GetHeight() const { return m_data.height; }

		virtual void Init(WindowProp) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void shutDown() = 0;
		virtual void onUpdate() = 0;

		virtual ~Window() = default;

		struct WindowData
		{
			int width;
			int height;
			CallBackFn callbackFn;

		};

		WindowData m_data;

		
	};
}