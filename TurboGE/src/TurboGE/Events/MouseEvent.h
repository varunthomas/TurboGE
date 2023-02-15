#pragma once
#include"Event.h"

namespace TurboGE
{
	class MousePressEvent : public Event
	{
		int m_button;
	public:
		MousePressEvent(int);
		//void dispatchEvent(MousePressEvent& e, std::function<void(MousePressEvent&)>) override;
		EventType getEventType() override;
		std::string getString() override;
		int getMouseButton();

	};

	class MouseReleaseEvent : public Event
	{
		int m_button;
	public:
		MouseReleaseEvent(int);
		//void dispatchEvent(MouseReleaseEvent& e, std::function<void(MouseReleaseEvent&)>) override;
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
		//void dispatchEvent(MouseScrollEvent& e, std::function<void(MouseScrollEvent&)>) override;
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
		//void dispatchEvent(MouseMoveEvent& e, std::function<void(MouseMoveEvent&)>) override;
		EventType getEventType() override;
		std::string getString() override;
	};
}
