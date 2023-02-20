#pragma once
#include"Event.h"

namespace TurboGE
{
	class KeyboardPressEvent : public Event
	{
		int m_key;
		int m_repeat;
	public:
		KeyboardPressEvent(int key, int repeat);
		//void dispatchEvent(KeyboardPressEvent& e, std::function<void(KeyboardPressEvent&)>) override;
		EventType getEventType() override;
		std::string getString() override;

		inline int getKey() { return m_key; }

	};

	class KeyboardReleaseEvent : public Event
	{
		int m_key;
	public:
		KeyboardReleaseEvent(int key);
		//void dispatchEvent(KeyboardReleaseEvent& e, std::function<void(KeyboardReleaseEvent&)>) override;
		EventType getEventType() override;
		inline int getKey() { return m_key; }
		std::string getString() override;
	};

	class KeyboardRepeatEvent : public Event
	{
		int m_key;
		int m_repeat;
	public:
		KeyboardRepeatEvent(int key, int repeat);
		//void dispatchEvent(KeyboardRepeatEvent& e, std::function<void(KeyboardRepeatEvent&)>) override;
		EventType getEventType() override;
		std::string getString() override;
	};

	class KeyboardTypedEvent : public Event
	{
		unsigned int m_key;
	public:
		KeyboardTypedEvent(unsigned int key);
		//void dispatchEvent(KeyboardPressEvent& e, std::function<void(KeyboardPressEvent&)>) override;
		EventType getEventType() override;
		inline int getKey() { return m_key; }
		std::string getString() override;

	};
}