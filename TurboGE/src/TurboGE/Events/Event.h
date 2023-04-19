#pragma once
#include"TurboGE/Core.h"

namespace TurboGE
{

	enum class EventType
	{
		MousePressEvent = 0, MouseReleaseEvent, MouseMoveEvent, MouseScrollEvent,
		KeyboardPressEvent, KeyboardReleaseEvent, KeyboardRepeatEvent, KeyboardTypedEvent,
		WindowCloseEvent, WindowSizeEvent
	};

	class Event
	{
	public:
		virtual EventType getEventType() = 0;
		virtual std::string getString() = 0;
		virtual ~Event() = default;

	};
}