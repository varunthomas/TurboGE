#pragma once
#include"Event.h"

namespace TurboGE
{
	class MousePressEvent : public Event
	{
		int m_button;
	public:
		MousePressEvent(int);
		EventType getEventType() override;
		std::string getString() override;
		int getMouseButton();

	};

	class MouseReleaseEvent : public Event
	{
		int m_button;
	public:
		MouseReleaseEvent(int);
		EventType getEventType() override;
		int getMouseButton();
		std::string getString() override;
	};

	class MouseScrollEvent : public Event
	{
		float m_xOffset;
		float m_yOffset;
	public:
		MouseScrollEvent(float, float);
		EventType getEventType() override;
		std::string getString() override;
		float getXOffset();
		float getYOffset();
	};

	class MouseMoveEvent : public Event
	{
		float m_xPos;
		float m_yPos;

	public:
		MouseMoveEvent(float, float);
		float getX();
		float getY();
		EventType getEventType() override;
		std::string getString() override;
	};
}
