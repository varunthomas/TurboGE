#pragma once
#include<memory>
#include"Renderer/Camera.h"
#include"Time.h"
#include"Input.h"
#include"Events/Event.h"
#include"Events/MouseEvent.h"
#include"Events/AppEvent.h"

namespace TurboGE
{
	class OrthographicCameraController
	{
		float m_ZoomLevel = 1.0f;
		float m_aspxRatio;
		Camera m_Camera;
		glm::vec3 m_CameraPos = { 0.5f, 0.5f, 0.0f };
		float m_CameraRot = 0.0f;
	public:
		OrthographicCameraController(float);

		void OnUpdate(Time);
		void onEvent(Event&);
		inline Camera GetCamera() { return m_Camera; }
		void onWindowResize(WindowSizeEvent&);
		void onMouseScrollEvent(MouseScrollEvent&);
		void onResize(float, float);

		~OrthographicCameraController() = default;
	};
}