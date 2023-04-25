#include"tgepch.h"
#include"BrowserPanel.h"
#include<imgui/imgui.h>

namespace TurboGE
{
	BrowserPanel::BrowserPanel()
		:m_Root{ std::filesystem::current_path()}, m_Current{m_Root}, m_FolderIcon{Texture2D::Create("assets/icons/folderIcon.png")}, m_FileIcon{Texture2D::Create("assets/icons/fileIcon.png")}
	{}

	void BrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Browser Panel");
		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		

		if (m_Root != m_Current)
		{
			if (ImGui::Button("<-"))
			{
				m_Current = m_Current.parent_path();
			}
		}

		uint8_t id = 0;
		ImGui::Columns(columnCount, 0, false);
		for (const auto& entry : std::filesystem::directory_iterator(m_Current))
		{
			
			id++;
			ImGui::PushID(id); //USED TO IDENTIFY EACH FILE SEPERATELY. IF NOT USED, THEN DRAG AND DROP WONT WORK AS EVERY FILE HAS SAME ICON
			
			auto iconID = entry.is_directory() ? m_FolderIcon->GetID() : m_FileIcon->GetID();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)iconID, { 128.0f, 128.0f });
			if (entry.is_directory() && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked((ImGuiMouseButton)0)) //DOUBLE CLICK DIRECTORY
			{
				m_Current /= entry.path().filename().string().c_str();
			}
			else if (ImGui::BeginDragDropSource()) //CLICK AND DRAG .TURBO FILE
			{
				std::filesystem::path file = m_Current / entry.path().filename().string().c_str();
				auto length = std::strlen(file.string().c_str()) + 1;
				auto itemPath = std::make_unique<char[]>(length); // +1 for the null terminator
				std::strcpy(itemPath.get(), file.string().c_str());
				ImGui::SetDragDropPayload("DRAG_DROP", itemPath.get(), length * sizeof(char));
				ImGui::EndDragDropSource();
			}
			ImGui::PopStyleColor();
			ImGui::TextWrapped(entry.path().filename().string().c_str());

			
			ImGui::NextColumn();
			ImGui::PopID();	
		}
		ImGui::Columns(1);
		ImGui::End();
	}
}