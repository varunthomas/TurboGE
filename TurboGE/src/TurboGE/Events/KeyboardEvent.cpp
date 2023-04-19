#include"tgepch.h"
#include"KeyboardEvent.h"
#include"TurboGE/Logger.h"

namespace TurboGE
{
	KeyboardPressEvent::KeyboardPressEvent(int key, int repeat)
	{
		m_key = key;
		m_repeat = repeat;
	}

	EventType KeyboardPressEvent::getEventType()
	{
		return EventType::KeyboardPressEvent;
	}

	std::string KeyboardPressEvent::getString()
	{
		std::stringstream ss;
		ss << "KeyboardPressed " << m_key << " Repeat " << m_repeat << std::endl;
		return ss.str();
	}

	KeyboardReleaseEvent::KeyboardReleaseEvent(int key)
	{
		m_key = key;
	}

	EventType KeyboardReleaseEvent::getEventType()
	{
		return EventType::KeyboardReleaseEvent;
	}

	std::string KeyboardReleaseEvent::getString()
	{
		std::stringstream ss;
		ss << "KeyboardReleased " << m_key << std::endl;
		return ss.str();
	}

	KeyboardRepeatEvent::KeyboardRepeatEvent(int key, int repeat)
	{
		m_key = key;
		m_repeat = repeat;
	}

	EventType KeyboardRepeatEvent::getEventType()
	{
		return EventType::KeyboardRepeatEvent;
	}

	std::string KeyboardRepeatEvent::getString()
	{
		std::stringstream ss;
		ss << "KeyboardRepeat " << m_key << " Repeat " << m_repeat << std::endl;
		return ss.str();
	}

	KeyboardTypedEvent::KeyboardTypedEvent(unsigned int key)
	{
		m_key = key;
	}

	EventType KeyboardTypedEvent::getEventType()
	{
		return EventType::KeyboardTypedEvent;
	}

	std::string KeyboardTypedEvent::getString()
	{
		std::stringstream ss;
		ss << "KeyboardTyped " << m_key <<  std::endl;
		return ss.str();
	}
}