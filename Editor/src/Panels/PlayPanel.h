#pragma once
#include"TurboGE/Renderer/Texture.h"

namespace TurboGE
{
	class PlayPanel
	{
		std::shared_ptr<Texture2D> m_IconPlay;
		std::shared_ptr<Texture2D> m_IconStop;
		
		
	public:
		PlayPanel();
		void OnImGuiRender();
		bool isPlay;
		bool toggle = false;
		~PlayPanel() = default;
	};
}