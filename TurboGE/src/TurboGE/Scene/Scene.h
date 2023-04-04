#pragma once
#include"entt.hpp"
#include"TurboGE/Time.h"
namespace TurboGE
{

	class Entity;
	
	class Scene
	{
		entt::registry m_registry;
	public:
		Scene() = default;
		~Scene();
		Entity CreateEntity(std::string_view str = "");
		void onUpdate(Time& t);

		friend class Entity;
	};


}