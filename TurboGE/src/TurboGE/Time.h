#pragma once

class Time
{
	float m_time;
public:
	Time(float time)
		:m_time{ time } {};

	operator float() const { return m_time; }
};