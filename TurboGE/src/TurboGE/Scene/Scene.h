#pragma once
#include<memory>
#include"entt.hpp"
#include"TurboGE/Time.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Physics2D.h"
namespace TurboGE
{
	class Entity;
	
	class Scene
	{
		entt::registry m_registry;
		Renderer2D& renderer2DInstance = Renderer2D::getInstance();
		glm::vec2 m_ViewportSize;
		void DrawRectVisualizer(const entt::entity, const glm::vec3&, const glm::vec3&, const glm::vec3&);
		void DrawCircleVisualizer(const entt::entity, const glm::vec3&, const glm::vec3&);
	public:
		Scene(glm::vec2&);
		~Scene() = default;
		Entity CreateEntity(std::string_view str = "");
		void DestroyEntity(entt::entity);
		void onUpdatePlay(Time& t, std::shared_ptr<Physics2D>&, bool);
		void onUpdateEditor(Time&, EditorCamera&, bool);
		void OnResize(uint32_t, uint32_t);
		Entity GetPrimaryCameraEntity();
		bool zoomed = false;
		float yoffsetScroll;

		template<typename T>
		void OnComponentAdded(T&);

		static void StartPhysics();

		friend class Entity;
		friend class EntityPanel;
		friend class SceneSerializer;
		friend class Physics2D;
	};


}