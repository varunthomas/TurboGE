#pragma once
#include"entt.hpp"
#include"TurboGE/Time.h"
#include"TurboGE/Renderer/Renderer2D.h"
namespace TurboGE
{

	class Entity;
	
	class Scene
	{
		entt::registry m_registry;
		Renderer2D& renderer2DInstance = Renderer2D::getInstance();
	public:
		Scene() = default;
		~Scene();
		Entity CreateEntity(std::string_view str = "");
		void onUpdate(Time& t);
		void OnResize(uint32_t, uint32_t);

		friend class Entity;
		friend class EntityPanel;
	};


}