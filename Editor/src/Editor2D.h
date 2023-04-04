#pragma once
#include<memory>
#include"TurboGE/Renderer/Renderer.h"
#include"Time.h"
#include"TurboGE/Events/Event.h"
#include"TurboGE/CameraController.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include"TurboGE/Renderer/Texture.h"
#include"TurboGE/Renderer/SubTexture2D.h"
#include"TurboGE/Scene/Scene.h"
#include"TurboGE/Renderer/FrameBuffer.h"
#include"TurboGE/Scene/EntityWrapper.h"
//TEMPORARY UNTIL I UNDERSTAND WHY LAYERS ARE USED

namespace TurboGE
{
	class Editor2D
	{
		std::unique_ptr<Scene> m_Scene;
		Entity m_SquareEntity;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Renderer2D& renderer2DInstance = Renderer2D::getInstance();
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<FrameBuffer> m_FrameBuffer;

		OrthographicCameraController m_CameraController{ 1280.0f / 720.0f };
		std::shared_ptr<Texture2D> m_CheckTexture;
		std::shared_ptr<Texture2D> m_SpriteSheet;
		std::unique_ptr<SubTexture2D> m_SpriteStairs;
		std::unique_ptr<SubTexture2D> m_SpriteBarrel;
		std::unique_ptr<SubTexture2D> m_SpriteTree;

	public:
		Editor2D();
		void OnAttach();
		void onUpdate(Time);
		void renderCustom();
		void onEvent(Event&);
		Editor2D* getLayer();
		~Editor2D();
	};

}