#include"tgepch.h"
#include"Sandbox2D.h"
#include"imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include"TurboGE/Events/AppEvent.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include<chrono>



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
	TGE_PROFILE_FUNCTION();
	{
		TGE_PROFILE_SCOPE("Render prep")
		m_Renderer->setClearColor();
		m_Renderer->Clear();
	}
	{
		TGE_PROFILE_SCOPE("Camera Controller");
		m_CameraController.OnUpdate(delta);
	}
	{
		TGE_PROFILE_SCOPE("Draw Render");
		renderer2DInstance.StartScene(m_CameraController.GetCamera());
		renderer2DInstance.DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		renderer2DInstance.DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//renderer2DInstance.DrawQuad({ 2.0f, 2.0f }, { 10.0f, 10.0f }, m_CheckTexture);
		renderer2DInstance.EndScene();
	}
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
	TGE_PROFILE_FUNCTION();
	ImGui::Begin("Color settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
