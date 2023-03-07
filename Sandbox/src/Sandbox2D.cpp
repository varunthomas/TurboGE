#include"tgepch.h"
#include"Sandbox2D.h"
#include"imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include"TurboGE/Events/AppEvent.h"
#include"TurboGE/Renderer/Renderer2D.h"

Sandbox2D::Sandbox2D()
{
	m_Renderer.reset(TurboGE::Renderer::Create());
	m_Renderer->Init();
	renderer2DInstance.Init();

	OnAttach();

}

void Sandbox2D::OnAttach()
{
	m_CheckTexture = TurboGE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onUpdate(TurboGE::Time delta)
{
	m_Renderer->setClearColor();
	m_Renderer->Clear();

	m_CameraController.OnUpdate(delta);

	renderer2DInstance.StartScene(m_CameraController.GetCamera());
	renderer2DInstance.DrawQuad({ -1.0f, -1.0f }, { 0.5f, 1.0f }, m_SquareColor);
	renderer2DInstance.DrawQuad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	renderer2DInstance.DrawQuad({ 2.0f, 2.0f }, { 10.0f, 10.0f }, m_CheckTexture);
	
}

void Sandbox2D::onEvent(TurboGE::Event& e)
{
	//RENDER NEW AREAS AFTER SIZE IS INCREASED
	if (e.getEventType() == TurboGE::EventType::WindowSizeEvent)
	{
		auto& winEvent = dynamic_cast<TurboGE::WindowSizeEvent&>(e);
		m_Renderer->setViewPort(winEvent.GetWidth(), winEvent.GetHeight());
	}

	m_CameraController.onEvent(e);
}

void Sandbox2D::renderCustom()
{
	ImGui::Begin("Color settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
