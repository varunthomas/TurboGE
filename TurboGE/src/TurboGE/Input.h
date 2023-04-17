#pragma once
#include"KeyCode.h"
#include"MouseCode.h"

namespace TurboGE
{
	class Input
	{
	public:
		static bool isKeyPressed(Key);
		
		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}