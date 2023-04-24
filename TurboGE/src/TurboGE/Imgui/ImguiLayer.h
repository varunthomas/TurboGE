#pragma once
#include"TurboGE/Core.h"
#include"TurboGE/Events/MouseEvent.h"
#include"TurboGE/Events/KeyboardEvent.h"
#include"TurboGE/Events/AppEvent.h"

namespace TurboGE
{

	class ImguiLayer
	{
		float m_Time = 0.0f;
	public:
		ImguiLayer() = default;
		void onAttach();
		void Begin();
		void End();
		inline void onEvent(Event& e) {};

		void SetDarkThemeColors();



		~ImguiLayer() = default;
	};

}