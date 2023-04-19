#include"tgepch.h"
#include"Camera.h"
#include"TurboGE/Events/MouseEvent.h"
#include"TurboGE/Input.h"
#include<glm/gtc/matrix_transform.hpp>
namespace TurboGE
{
	void Camera::setPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateMatrices();
	}

	void Camera::setRotation(float rotation)
	{
		m_Rotation = rotation;
		RecalculateMatrices();
	}

	void Camera::RecalculateMatrices()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::setProjection(float left, float right, float down, float up)
	{
		m_ProjectionMatrix = glm::ortho(left, right, down, up, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



	//******************GAME CAMERA******************

	GameCamera::GameCamera()
	{
		RecalculateProjection();
	}

	void GameCamera::RecalculateProjection()
	{
		if (m_ProjectionType == Projection::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;


			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}


	//EDITOR CAMERA

	EditorCamera::EditorCamera(float aspectRatio)
		:m_AspectRatio{aspectRatio}
	{
	}

	void EditorCamera::OnUpdate(Time& t)
	{
		xpos = Input::GetMouseX();
		ypos = Input::GetMouseY();
		
		if (Input::isKeyPressed(Key::LeftAlt) && Input::isMouseButtonPressed(MouseCode::ButtonLeft))
		{
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			xoffset = xpos - lastX;
			yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.1f; // change this value to your liking
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
			UpdateDirection();
			
		}
		else
		{
			xoffset = xpos - lastX;
			yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;
		}
			cameraSpeed = 2.0f * t;
			if (Input::isKeyPressed(Key::W))
			{
				cameraPos += cameraSpeed * cameraFront;
			}
			if (Input::isKeyPressed(Key::S))
				cameraPos -= cameraSpeed * cameraFront;
			if (Input::isKeyPressed(Key::A))
				cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			if (Input::isKeyPressed(Key::D))
				cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			m_View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			RecalculateProjection();
			
		
	}


	void EditorCamera::OnEvent(Event& e)
	{
		auto& mouseScrollEvent = dynamic_cast<MouseScrollEvent&>(e);
		float yoffsetScroll = mouseScrollEvent.getYOffset();
		SetZoomFoV(yoffsetScroll);

		if (e.getEventType() == EventType::MousePressEvent)
		{
			m_MousePressed = true;
		}
		if (e.getEventType() == EventType::MouseReleaseEvent)
		{
			m_MousePressed = false;
		}

	}

	void EditorCamera::SetZoomFoV(float yoffsetScroll)
	{
		m_Fov -= yoffsetScroll;
		if (m_Fov < 1.0f)
			m_Fov = 1.0f;
		if (m_Fov > 60.0f)
			m_Fov = 60.0f;
	}

	void EditorCamera::UpdateDirection()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height; RecalculateProjection();
	}

	void EditorCamera::RecalculateProjection()
	{
		m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, 0.1f, 100.0f);
	}
}
