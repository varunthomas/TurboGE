#pragma once
#include<memory>
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Renderer.h"
#include"Time.h"
#include"TurboGE/Events/Event.h"
#include"TurboGE/CameraController.hpp"

//TEMPORARY UNTIL I UNDERSTAND WHY LAYERS ARE USED
class Sandbox2D
{

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f};

	std::unique_ptr<TurboGE::VertexArray> m_SquareVA;
	std::unique_ptr<TurboGE::VertexBuffer> m_SquareVB;
	std::shared_ptr<TurboGE::IndexBuffer> m_SquareIB;
	std::unique_ptr<TurboGE::Shader> m_SquareShader;

	std::unique_ptr<TurboGE::Renderer> m_Renderer;

	TurboGE::OrthographicCameraController m_CameraController{ 1280.0f / 720.0f };

public:
	Sandbox2D();
	void onUpdate(TurboGE::Time);
	void renderCustom();
	void onEvent(TurboGE::Event&);

};