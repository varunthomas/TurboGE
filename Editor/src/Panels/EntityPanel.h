#pragma once
#include<memory>
#include"TurboGE/Scene/Scene.h"
#include"TurboGE/Scene/EntityWrapper.h"
namespace TurboGE
{
	class EntityPanel
	{
		std::shared_ptr<Scene> m_Scene;
		Entity m_SelectionContext{};
	public:
		EntityPanel() = default;
		void operator()(const std::shared_ptr<Scene>&);
		void OnImGuiRender();
		void DrawEntityList(const Entity&);
		void DrawPropertiesPanel(const Entity&);
		Entity GetSelectedEntity() { return m_SelectionContext; }
		void SetSelectedEntity(Entity e) { m_SelectionContext = e; }
	};
}