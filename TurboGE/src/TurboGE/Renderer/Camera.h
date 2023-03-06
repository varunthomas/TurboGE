#pragma once
#include<glm/glm.hpp>

namespace TurboGE
{
	class OrthographicCamera
	{
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	public:
		OrthographicCamera(float, float, float, float);
		~OrthographicCamera() = default;

		void setPosition(const glm::vec3&);
		void setRotation(float);
		inline glm::mat4 getViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
		void setProjection(float, float, float, float);
		void RecalculateMatrices();
	};
}