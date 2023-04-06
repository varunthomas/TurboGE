
#include"EntityPanel.h"
#include<imgui/imgui.h>
#include"TurboGE/Scene/Components.h"

namespace TurboGE
{

	void EntityPanel::operator()(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
	}
	void EntityPanel::OnImGuiRender()
	{
		ImGui::Begin("Entity Panel");

		m_Scene->m_registry.each([&](auto entity)
			{
				
				Entity e{ entity, m_Scene.get() };
				auto& tag = e.GetComponent<TagComponent>().tag;

				ImGuiTreeNodeFlags flags = ((m_SelectionContext == e) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectionContext = e;
				}
				if (opened)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
					if (opened)
						ImGui::TreePop();
					ImGui::TreePop();
				}
			});
		

		ImGui::End();
	}
}