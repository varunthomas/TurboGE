#include"tgepch.h"
#include"AppEvent.h"
#include"TurboGE/Logger.h"

namespace TurboGE
{
	WindowCloseEvent::WindowCloseEvent()
	{
		TURBO_CORE_ERR("Closed Window");
	};

	EventType WindowCloseEvent::getEventType()
	{
		return EventType::WindowCloseEvent;
	}

	std::string WindowCloseEvent::getString()
	{
		std::stringstream ss;
		ss << "WindowCloseEvent " << std::endl;
		return ss.str();
	}

	WindowSizeEvent::WindowSizeEvent(unsigned int width, unsigned int height)
		:m_width{ width }, m_height{ height } {};

	

	EventType WindowSizeEvent::getEventType()
	{
		return EventType::WindowSizeEvent;
	}

	std::string WindowSizeEvent::getString()
	{
		std::stringstream ss;
		ss << "WindowSizeEvent width " << m_width << " height " << m_height << std::endl;
		return ss.str();
	}
}