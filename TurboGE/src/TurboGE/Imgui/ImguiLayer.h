#pragma once
#include"TurboGE/Core.h"
#include"TurboGE/Events/MouseEvent.h"
#include"TurboGE/Events/KeyboardEvent.h"
#include"TurboGE/Events/AppEvent.h"
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
		inline void onEvent(Event& e) {};


		~ImguiLayer();
	};

}