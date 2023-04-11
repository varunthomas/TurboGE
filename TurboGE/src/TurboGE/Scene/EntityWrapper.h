#pragma once
#include"entt.hpp"
#include"Scene.h"
#include<iostream>
namespace TurboGE
{
	class Entity
	{
		entt::entity entityID{ entt::null };
		Scene* m_Scene{};
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene)
			:entityID{ entity }, m_Scene{ scene } {}
		~Entity() = default;

		entt::entity GetID() const
		{
			return entityID;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_registry.emplace<T>(entityID, std::forward<Args>(args)...);
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
			//std::cout << "uint32 typecast\n";
			return (uint32_t)entityID;
		}

		operator entt::entity() const 
		{
			std::cout << "entt::entity typecast\n";
			return entityID;
		}

		operator bool() const
		{
			return entityID != entt::null;
		}

		auto operator<=>(const Entity&) const = default;
	};
}