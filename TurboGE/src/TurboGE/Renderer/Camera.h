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

	class GameCamera
	{
		glm::mat4 m_Projection;
		float m_AspectRatio{ 0.0f };
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
	public:
		GameCamera();
		~GameCamera() = default;
		//GameCamera(const glm::mat4& projection)
			//: m_Projection(projection) {}

		const glm::mat4& GetProjection() const { return m_Projection; }
		void RecalculateProjection();
		void SetViewPort(uint32_t width, uint32_t height) { m_AspectRatio = (float)width / (float)height; RecalculateProjection(); }
		float GetOrthoSize() { return m_OrthographicSize; }
		void SetOrthoSize(float orthoSize) { m_OrthographicSize = orthoSize; RecalculateProjection(); }

	};
}