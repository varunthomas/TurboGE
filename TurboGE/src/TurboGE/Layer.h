#pragma once

#include "TurboGE/Events/Event.h"
#include "TurboGE/Time.h"

namespace TurboGE
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void onUpdate(Time) {}
		virtual void renderCustom() {}
		virtual void onEvent(Event&) {}
	};
}