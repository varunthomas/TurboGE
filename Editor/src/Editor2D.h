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
#include"TurboGE/Physics2D.h"

#include"Panels/BrowserPanel.h"
#include"Panels/EntityPanel.h"
#include"Panels/PlayPanel.h"
#include<iostream>

namespace TurboGE
{
	class Editor2D
	{
		std::shared_ptr<Scene> m_Scene;
		EntityPanel entityPanel;
		BrowserPanel browserPanel;
		PlayPanel playPanel;

		std::shared_ptr<Physics2D> m_Physics;

		Entity m_ClickedEntity;

		void LoadScene();
		void LoadScene(const std::string&);
		void NewScene();
		void SaveScene();
		void SaveScene(const std::string&);
		void Deserialize();
		std::string Serialize();
		std::string m_CurrentSceneFile;
		std::string tempData = std::string();

		int m_TransformGizmo{ -1 };
		bool m_Snap = false;

		bool m_ShowPhysicsColliders = false;

		bool m_DialogDone = true;
		bool m_RestoreDown = true;

		Entity m_SquareEntity;
		Entity m_Camera;
		Entity m_SecCamera;

		bool m_PrimaryCamera = true;
		EditorCamera m_EditorCamera{1920.0f/1080.0f};

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		std::array<glm::vec2, 2> m_BoundsArray;

		Renderer2D& renderer2DInstance = Renderer2D::getInstance();
		std::unique_ptr<Renderer> m_Renderer;
		std::unique_ptr<FrameBuffer> m_FrameBuffer;

	public:
		Editor2D();
		void OnAttach();
		void onUpdate(Time);
		void renderCustom();
		void onEvent(Event&);
		Editor2D* getLayer();
		~Editor2D() { std::cout << "Dest editor\n"; };
	};

}