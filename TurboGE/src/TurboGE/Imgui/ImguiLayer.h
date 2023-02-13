#pragma once
#include"TurboGE/Core.h"
#include"TurboGE/Events/MouseEvent.h"
#include"TurboGE/Events/KeyboardEvent.h"
#include"TurboGE/Events/AppEvent.h"
#include"GLFW/glfw3.h"
#include"TurboGE/Logger.h"

namespace TurboGE
{

	class TGE_API ImguiLayer
	{
		float m_Time = 0.0f;
	public:
		ImguiLayer();
		void onUpdate();
		void onAttach();
		void onEvent(Event& e);
		void onMousePressEvent(MousePressEvent& e);
		void onMouseReleaseEvent(MouseReleaseEvent& e);
		void onMouseMoveEvent(MouseMoveEvent& e);
		void onMouseScrollEvent(MouseScrollEvent& e);

		void onKeyboardPressEvent(KeyboardPressEvent& e);
		void onKeyboardReleaseEvent(KeyboardReleaseEvent& e);
		void onKeyboardTypedEvent(KeyboardTypedEvent& e);

		void OnWindowResizeEvent(WindowSizeEvent& e);


		~ImguiLayer();
	};

}