#pragma once
#include"KeyCode.h"
#include"MouseCode.h"

namespace TurboGE
{
	class Input
	{
	public:

		enum class InputViewport
		{
			EDITOR = 1,
			PANEL
		};

		static inline InputViewport inputViewPort = InputViewport::EDITOR;

		static bool isKeyPressed(Key);
		
		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}