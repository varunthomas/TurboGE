#include"tgepch.h"
#include"Camera.h"
#include<glm/gtc/matrix_transform.hpp>
namespace TurboGE
{
	OrthographicCamera::OrthographicCamera(float left, float right, float down, float up)
		:m_ProjectionMatrix{glm::ortho(left, right, down, up, -1.0f, 1.0f)},m_ViewMatrix{1.0f}
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::setPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateMatrices();
	}

	void OrthographicCamera::setRotation(float rotation)
	{
		m_Rotation = rotation;
		RecalculateMatrices();
	}

	void OrthographicCamera::RecalculateMatrices()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float down, float up)
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
		//std::cout << "Recalculating\n";
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;


		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}
