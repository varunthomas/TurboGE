#pragma once
#include"entt.hpp"
#include"Scene.h"
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

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_registry.emplace<T>(entityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Scene->m_registry.get<T>(entityID);
		}

		operator uint32_t() const
		{
			return (uint32_t)entityID;
		}

		operator bool() const
		{
			return entityID != entt::null;
		}

		auto operator<=>(const Entity&) const = default;
	};
}