#pragma once
#include<glm/glm.hpp>
#include"TurboGE/Events/Event.h"
#include"TurboGE/Time.h"

namespace TurboGE
{
	class Camera
	{
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	public:
		~Camera() = default;

		void setPosition(const glm::vec3&);
		void setRotation(float);
		inline glm::mat4 getViewProjectionMatrix() const { return m_ViewProjectionMatrix; };
		void setProjection(float, float, float, float);
		void RecalculateMatrices();
	};

	
	//GAME CAMERA

	enum class Projection
	{
		Perspective,
		Orthographic
	};
	class GameCamera
	{
		
	private:
		glm::mat4 m_Projection;
		float m_AspectRatio{ 0.0f };
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		Projection m_ProjectionType = Projection::Perspective;

	public:
		
		GameCamera();
		~GameCamera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
		void RecalculateProjection();
		void SetViewPort(uint32_t width, uint32_t height) { m_AspectRatio = (float)width / (float)height; RecalculateProjection(); }
		float GetOrthoSize() { return m_OrthographicSize; }
		void SetOrthoSize(float orthoSize) { m_OrthographicSize = orthoSize; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		Projection GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(Projection projection) { m_ProjectionType = projection; RecalculateProjection(); }

	};

	//EDITOR CAMERA

	class EditorCamera
	{
		
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_Projection;
		float m_AspectRatio;

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float cameraSpeed = 2.0f; // adjust accordingly

		bool firstMouse = true;
		float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		float pitch = 0.0f;
		float lastX = 1280.0f / 2.0f; //MIDDLE OF SCREEN
		float lastY = 720.0f / 2.0f;
		float m_Fov = 45.0f;
		bool m_MousePressed = false;
		float xpos, ypos;
		float xoffset, yoffset;

	public:

		EditorCamera(float);
		void OnEvent(Event&);
		void OnUpdate(Time&);
		void SetZoomFoV(float);
		void UpdateDirection();
		void SetViewportSize(uint32_t, uint32_t);
		glm::mat4 GetViewProjection() { return m_Projection * m_View; }
		glm::mat4 GetView() { return m_View; }
		glm::mat4 GetProjection() { return m_Projection; }
		void RecalculateProjection();
		~EditorCamera() = default;
	};
}