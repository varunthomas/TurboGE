#pragma once
#include"TurboGE/Renderer/Texture.h"
namespace TurboGE
{
	class BrowserPanel
	{
		std::filesystem::path m_Root;
		std::filesystem::path m_Current;

		std::shared_ptr<Texture2D> m_FileIcon;
		std::shared_ptr<Texture2D> m_FolderIcon;
		public:
			BrowserPanel();
			void OnImGuiRender();
	};
}