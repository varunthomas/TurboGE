#include"tgepch.h"
#include"Application.h"
#include"Input.h"
#include<GLFW/glfw3.h>


namespace TurboGE
{
	bool Input::isKeyPressed(Key keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
}