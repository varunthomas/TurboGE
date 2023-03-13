#include"tgepch.h"
#include"Sandbox2D.h"
#include"imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include"TurboGE/Events/AppEvent.h"
#include"TurboGE/Renderer/Renderer2D.h"
//#include"TurboGE/Application.h"
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
	m_SpriteSheet = TurboGE::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_SpriteStairs = TurboGE::SubTexture2D::Create(m_SpriteSheet, { 7,6 }, { 128, 128 }, { 1,1 });
	m_SpriteBarrel = TurboGE::SubTexture2D::Create(m_SpriteSheet, { 8,2 }, { 128, 128 }, { 1,1 });
	m_SpriteTree = TurboGE::SubTexture2D::Create(m_SpriteSheet, { 2,1 }, { 128, 128 }, {1,2});
}

void Sandbox2D::onUpdate(TurboGE::Time delta)
{
	TGE_PROFILE_FUNCTION();
	renderer2DInstance.ResetStats();
	{
		TGE_PROFILE_SCOPE("Render prep");
		m_Renderer->setClearColor();
		m_Renderer->Clear();
	}
	{
		TGE_PROFILE_SCOPE("Camera Controller");
		m_CameraController.OnUpdate(delta);
	}
	{
		TGE_PROFILE_SCOPE("Draw Render");
#if 0
		renderer2DInstance.StartScene(m_CameraController.GetCamera());
		renderer2DInstance.DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		renderer2DInstance.DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		renderer2DInstance.DrawQuad({ -5.0f, -5.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckTexture, 10.0f);
		//renderer2DInstance.DrawQuad({ 2.0f, 2.0f }, { 10.0f, 10.0f }, m_CheckTexture);

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				renderer2DInstance.DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		renderer2DInstance.EndScene();
#endif

		renderer2DInstance.StartScene(m_CameraController.GetCamera());
		//renderer2DInstance.DrawQuad({ 0.0f, 0.0f, 0.2f}, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f);
		
		renderer2DInstance.DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, m_SpriteStairs, 1.0f);
		renderer2DInstance.DrawQuad({ 1.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, m_SpriteBarrel, 1.0f);
		renderer2DInstance.DrawQuad({ 2.0f, 0.0f, 0.2f }, { 1.0f, 2.0f }, m_SpriteTree, 1.0f);
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
    ImGui::Text("Draw calls: %d", renderer2DInstance.GetStats().drawCalls);
    ImGui::Text("Quad count: %d", renderer2DInstance.GetStats().quadCount);
    ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}



Sandbox2D::~Sandbox2D()
{
	std::cout << "Dlete Sandbox2D\n";
}
