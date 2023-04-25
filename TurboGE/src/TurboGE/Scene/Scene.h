#pragma once
#include"entt.hpp"
#include"TurboGE/Time.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include"TurboGE/Renderer/Camera.h"
namespace TurboGE
{

	class Entity;
	
	class Scene
	{
		entt::registry m_registry;
		Renderer2D& renderer2DInstance = Renderer2D::getInstance();
	public:
		Scene() = default;
		~Scene() = default;
		Entity CreateEntity(std::string_view str = "");
		void DestroyEntity(entt::entity);
		void onUpdatePlay(Time& t);
		void onUpdateEditor(Time&, EditorCamera&);
		void OnResize(uint32_t, uint32_t);
		Entity GetPrimaryCameraEntity();
		bool zoomed = false;
		float yoffsetScroll;

		friend class Entity;
		friend class EntityPanel;
		friend class SceneSerializer;
	};


}