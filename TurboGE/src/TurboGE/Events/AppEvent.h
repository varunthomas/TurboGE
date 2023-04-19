#pragma once
#include"Event.h"

namespace TurboGE
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent();
		EventType getEventType() override;
		std::string getString() override;

	};

	class WindowSizeEvent : public Event
	{
		unsigned int m_width;
		unsigned int m_height;
	public:
		WindowSizeEvent(unsigned int, unsigned int);

		inline unsigned int GetWidth() const { return m_width; }

		inline unsigned int GetHeight() const { return m_height; }

		EventType getEventType() override;
		std::string getString() override;

	};

}

