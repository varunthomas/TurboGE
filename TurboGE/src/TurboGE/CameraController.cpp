#include"tgepch.h"
#include"CameraController.h"
#include"GLFW/glfw3.h"

namespace TurboGE
{
	OrthographicCameraController::OrthographicCameraController(float aspxRatio)
		:m_aspxRatio{ aspxRatio } {};

	void OrthographicCameraController::OnUpdate(Time delta)
	{
		if (Input::isKeyPressed(Key::Up))
		{
			m_CameraPos.y += 5.0f * delta;
		}
		else if (Input::isKeyPressed(Key::Down))
		{
			m_CameraPos.y -= 5.0f * delta;
		}
		else if (Input::isKeyPressed(Key::Left))
		{
			m_CameraPos.x -= 5.0f * delta;
		}
		else if (Input::isKeyPressed(Key::Right))
		{
			m_CameraPos.x += 5.0f * delta;
		}
		else if (Input::isKeyPressed(Key::A))
		{
			m_CameraRot -= 4.0f * delta;
		}
		else if (Input::isKeyPressed(Key::D))
		{
			m_CameraRot += 4.0f * delta;
		}
		m_Camera.setPosition(m_CameraPos);
		m_Camera.setRotation(m_CameraRot);
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		if (e.getEventType() == EventType::MouseScrollEvent)
		{
			onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(e));
		}
		else if (e.getEventType() == EventType::WindowSizeEvent)
		{
			onWindowResize(dynamic_cast<WindowSizeEvent&>(e));
		}
	}

	void OrthographicCameraController::onMouseScrollEvent(MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.getYOffset() * 0.25f;

		if (m_ZoomLevel < 0.25f)
		{
			m_ZoomLevel = 0.25f;
		}
		else if (m_ZoomLevel > 3.0f)
		{
			m_ZoomLevel = 3.0f;
		}
		m_Camera.setProjection(-m_aspxRatio * m_ZoomLevel, m_aspxRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::onWindowResize(WindowSizeEvent& e)
	{
		m_aspxRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.setProjection(-m_aspxRatio * m_ZoomLevel, m_aspxRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::onResize(float width, float height)
	{
		m_aspxRatio = width / height;
		m_Camera.setProjection(-m_aspxRatio * m_ZoomLevel, m_aspxRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
}