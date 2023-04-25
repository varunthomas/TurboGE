
#include"EntityPanel.h"
#include<imgui/imgui.h>
#include<imgui/misc/cpp/imgui_stdlib.h>
#include"TurboGE/Scene/Components.h"
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<imgui/imgui_internal.h>

namespace TurboGE
{

	void EntityPanel::operator()(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectionContext = {};
	}

	template<typename T>
	static void DrawComponentPanel(const std::string& name, Entity entity, std::function<void(T&)> uiFunction)
	{
		if (entity.HasComponent<T>())
		{
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding, name.c_str()))
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
				{
					entity.RemoveComponent<T>();
					ImGui::TreePop();
					return;
				}
				auto& component = entity.GetComponent<T>();
				uiFunction(component);
				ImGui::TreePop();
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void EntityPanel::OnImGuiRender()
	{
		ImGui::Begin("Entity Panel");

		m_Scene->m_registry.each([&](auto entityID)
			{
				
				Entity entity{ entityID, m_Scene.get() };
				DrawEntityList(entity);
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete entity"))
					{
						m_Scene->DestroyEntity(entity);
						if(m_SelectionContext == entity)
							m_SelectionContext = {};
					}
					ImGui::EndPopup();
				}

			});
		
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}	

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create new entity"))
			{
				m_Scene->CreateEntity("Empty entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawPropertiesPanel(m_SelectionContext);
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("Add Component");
			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}

		ImGui::End();
	}

	void EntityPanel::DrawEntityList(const Entity& entity )
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void EntityPanel::DrawPropertiesPanel(const Entity& entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		std::string buffer = tag;
		if (ImGui::InputText("Tag", &buffer))
		{
			tag = buffer;
		}

		
		DrawComponentPanel<TransformComponent>("Transform", entity, [](auto& component) {
			DrawVec3Control("Translation", component.translate);
			glm::vec3 rotation = glm::degrees(component.rotate);
			DrawVec3Control("Rotation", rotation);
			component.rotate = glm::radians(rotation);
			DrawVec3Control("Scale", component.scale, 1.0f);

			});
		DrawComponentPanel<CameraComponent>("Camera", entity, [&](auto& component) {
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;
			if (ImGui::Checkbox("Primary", &(entity.GetComponent<CameraComponent>().primary)))
			{
				entity.CheckEach<CameraComponent>([&](auto& component, auto id)
					{
						if (entity.GetID() != id && component.primary == true)
						{
							component.primary = false;
						}
					});
			}

			std::array<const char*, 2> cameraType{ "Perspective", "Orthographic" };
			const char* currentProjection = cameraType[static_cast<size_t>(camera.GetProjectionType())];
			if (ImGui::BeginCombo("Projection", currentProjection))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjection == cameraType[i];
					if (ImGui::Selectable(cameraType[i], isSelected))
					{
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

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
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
			});

		DrawComponentPanel<SpriteRendererComponent>("Sprite renderer", entity, [](auto& component) {
			ImGui::ColorEdit4("Square color", glm::value_ptr(component.color));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_DROP"))
				{

					const char* path = (const char*)payload->Data;
					std::string loadFile(path);
					component.texture = Texture2D::Create(loadFile);

				}
				ImGui::EndDragDropTarget();
			}
		});
	}
}