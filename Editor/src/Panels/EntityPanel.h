#pragma once
#include<memory>
#include"TurboGE/Scene/Scene.h"
#include"TurboGE/Scene/EntityWrapper.h"
namespace TurboGE
{
	class EntityPanel
	{
		const std::unique_ptr<Scene>& m_Scene;
		Entity m_SelectionContext{};
		template <typename T>
		void DisplayPopup(const std::string&);
	public:
		EntityPanel(const std::unique_ptr<Scene>&);
		void OnImGuiRender();
		void DrawEntityList(const Entity&);
		void DrawPropertiesPanel(const Entity&);
		Entity GetSelectedEntity() { return m_SelectionContext; }
		void SetSelectedEntity(Entity e) { m_SelectionContext = e; }
	};
}