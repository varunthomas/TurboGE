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

	class TGE_API Event
	{
	public:

		//virtual int getMouseButton() = 0;
		//virtual double getX() = 0;
		//virtual double getY() = 0;
		virtual EventType getEventType() = 0;
		virtual std::string getString() = 0;
		virtual ~Event() = default;

		//template<typename T>
		//virtual void dispatchEvent(T& e, std::function<void(Event&)>) = 0;
	};

	//TGE_API not present for EventDispatcher in cherno code
	/*class TGE_API EventDispatcher
	{
		Event m_event;
	public:
		EventDispatcher(Event& e)
		{
			m_event = e;
		}
		template<typename T>
		void dispatch(std::function<void(T& )> imguiEvent)
		{
			if (m_event.getType() == T::getType())
			{
				imguiEvent()
			}
		}
	};*/
}