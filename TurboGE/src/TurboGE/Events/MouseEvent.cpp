#include"tgepch.h"
#include"MouseEvent.h"
#include"TurboGE/Logger.h"

namespace TurboGE
{
	MousePressEvent::MousePressEvent(int button)
		:m_button{ button } {
		std::cout << "Mouse pressed\n";
	};

	std::string MousePressEvent::getString()
	{
		std::stringstream ss;
		ss << "MousePressEvent " << m_button <<  std::endl;
		return ss.str();
	}

	EventType MousePressEvent::getEventType()
	{
		return EventType::MousePressEvent;
	}

	int MousePressEvent::getMouseButton()
	{
		return m_button;
	}

	MouseReleaseEvent::MouseReleaseEvent(int button)
		:m_button{ button } {};

	std::string MouseReleaseEvent::getString()
	{
		std::stringstream ss;
		ss << "MouseReleaseEvent " << m_button << std::endl;
		return ss.str();
	}

	EventType MouseReleaseEvent::getEventType()
	{
		return EventType::MouseReleaseEvent;
	}

	int MouseReleaseEvent::getMouseButton()
	{
		return m_button;
	}

	MouseScrollEvent::MouseScrollEvent(float xOffset, float yOffset)
		:m_xOffset{ xOffset }, m_yOffset{ yOffset } {};

	std::string MouseScrollEvent::getString()
	{
		std::stringstream ss;
		ss << "MouseScrollEvent x Offset " << m_xOffset << " y offset " << m_yOffset << std::endl;
		return ss.str();
	}

	EventType MouseScrollEvent::getEventType()
	{
		return EventType::MouseScrollEvent;
	}

	float MouseScrollEvent::getXOffset()
	{
		return m_xOffset;
	}

	float MouseScrollEvent::getYOffset()
	{
		return m_yOffset;
	}

	MouseMoveEvent::MouseMoveEvent(float xPos, float yPos)
		:m_xPos{ xPos }, m_yPos{ yPos } {};
	
	//void MouseMoveEvent::dispatchEvent(MouseMoveEvent e, std::function<void(MouseMoveEvent&)> func)
	//{
		//func(e);
	//}
	std::string MouseMoveEvent::getString()
	{
		std::stringstream ss;
		ss << "MouseMoveEvent x pos" << m_xPos << " y pos " << m_yPos << std::endl;
		return ss.str();
	}

	EventType MouseMoveEvent::getEventType()
	{
		return EventType::MouseMoveEvent;
	}

	float MouseMoveEvent::getX()
	{
		return m_xPos;
	}
	float MouseMoveEvent::getY()
	{
		return m_yPos;
	}
}