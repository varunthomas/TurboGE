#pragma once
#include"entt.hpp"
#include"Scene.h"

class b2Body;
namespace TurboGE
{
	class Entity
	{
		entt::entity entityID{ entt::null };
		Scene* m_Scene{};
		b2Body* m_body = nullptr;
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene)
			:entityID{ entity }, m_Scene{ scene } {}
		~Entity() = default;

		entt::entity GetID() const
		{
			return entityID;
		}

		template<typename T>
		std::string GetName()
		{
			T a;
			return a.name;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			T& component = m_Scene->m_registry.emplace<T>(entityID, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(component);
			return component;
		}

		template<typename T>
		void RemoveComponent() const
		{
			m_Scene->m_registry.remove<T>(entityID);
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Scene->m_registry.get<T>(entityID);
		}

		template<typename T>
		T* HasComponent() const
		{
			return m_Scene->m_registry.try_get<T>(entityID);
			
		}

		template<typename T>
		void CheckEach(std::function<void(T&, entt::entity)> modifier) const
		{
			auto group = m_Scene->m_registry.view<T>();
			for (auto entity : group)
			{
				auto& component = group.get<T>(entity);
				modifier(component, entity);
			}
		}

		operator uint32_t() const
		{
			return (uint32_t)entityID;
		}

		operator entt::entity() const 
		{
			return entityID;
		}

		operator int() const
		{
			return (int)entityID;
		}

		operator bool() const
		{
			return entityID != entt::null;
		}

		auto operator<=>(const Entity&) const = default;
	};
}