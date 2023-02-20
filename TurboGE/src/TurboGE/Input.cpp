#include"tgepch.h"
#include"Application.h"
#include"Input.h"
#include<GLFW/glfw3.h>

namespace TurboGE
{
	bool Input::isKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
}