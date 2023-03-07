#pragma once
#include<memory>
#include"TurboGE/Renderer/Renderer.h"
#include"Time.h"
#include"TurboGE/Events/Event.h"
#include"TurboGE/CameraController.hpp"
#include"TurboGE/Renderer/Renderer2D.h"

//TEMPORARY UNTIL I UNDERSTAND WHY LAYERS ARE USED
class Sandbox2D
{

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f};

	TurboGE::Renderer2D& renderer2DInstance = TurboGE::Renderer2D::getInstance();
	std::unique_ptr<TurboGE::Renderer> m_Renderer;
	TurboGE::OrthographicCameraController m_CameraController{ 1280.0f / 720.0f };

public:
	Sandbox2D();
	void onUpdate(TurboGE::Time);
	void renderCustom();
	void onEvent(TurboGE::Event&);

};