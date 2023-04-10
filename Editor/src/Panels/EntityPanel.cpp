
#include"EntityPanel.h"
#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>
#include"TurboGE/Scene/Components.h"
#include<glm/gtc/type_ptr.hpp>
#include<iostream>

namespace TurboGE
{

	void EntityPanel::operator()(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
	}
	void EntityPanel::OnImGuiRender()
	{
		ImGui::Begin("Entity Panel");

		m_Scene->m_registry.each([&](auto entityID)
			{
				
				Entity entity{ entityID, m_Scene.get() };
				DrawEntityList(entity);

			});
		
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawPropertiesPanel(m_SelectionContext);
		}

		

		ImGui::End();
	}

	void EntityPanel::DrawEntityList(const Entity& entity )
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
		/*if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}*/
	}

	void EntityPanel::DrawPropertiesPanel(const Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		std::string buffer = tag;
		//char buffer[256];
		if (ImGui::InputText("Tag", &buffer))
		{
			tag = buffer;
		}

		auto& transform = entity.GetComponent<TransformComponent>().transform;
		ImGui::DragFloat3("Transform", glm::value_ptr(transform[3]));

	
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().camera;
				ImGui::Checkbox("Primary", &(entity.GetComponent<CameraComponent>().primary));
				
				std::array<const char*, 2> cameraType {"Perspective", "Orthographic"};
				const char* currentProjection = cameraType[static_cast<size_t>(camera.GetProjectionType())];
				if (ImGui::BeginCombo("Projection", currentProjection))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjection == cameraType[i];
						if (ImGui::Selectable(cameraType[i], isSelected))
						{
							std::cout << "Selectable\n";
							currentProjection = cameraType[i];
							camera.SetProjectionType((Projection)i);
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
						
					}
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == Projection::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float orthoNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetPerspectiveNearClip(orthoNear);

					float orthoFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetPerspectiveFarClip(orthoFar);
				}
				else if (camera.GetProjectionType() == Projection::Orthographic)
				{
					float orthoSize = camera.GetOrthoSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthoSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);
				}
				ImGui::TreePop();
			}
		}

	}
}