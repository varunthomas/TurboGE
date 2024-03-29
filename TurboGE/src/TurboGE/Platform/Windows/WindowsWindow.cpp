#include"tgepch.h"
#include "WindowsWindow.h"
#include"TurboGE/Events/KeyboardEvent.h"
#include"TurboGE/Events/MouseEvent.h"
#include"TurboGE/Events/AppEvent.h"
#include"glad/glad.h"

namespace TurboGE
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		TURBO_CORE_ERR("GLFW Error ({0}): {1}", error, description);
	}
	std::unique_ptr<Window> TurboGE::Window::Create(WindowProp props)
	{
		return std::unique_ptr<Window>{new WindowsWindow(props)};
	}

	WindowsWindow::~WindowsWindow()
	{
		shutDown();
	}
	void TurboGE::WindowsWindow::Init(WindowProp props)
	{
		m_data.height = props.height;
		m_data.width = props.width;
		if (!m_Glfw_init)
		{
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			m_Glfw_init = true;
		}

		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);  // Disable window decorations
		glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
		
		m_window = glfwCreateWindow((int)props.width, (int)props.height, "TURBOGE", nullptr, nullptr);
		
		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		
		std::cout << glGetString(GL_VENDOR) << " " << glGetString(GL_VERSION) << " " << glGetString(GL_RENDERER) << std::endl;

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.callbackFn(event);
			});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowSizeEvent event(width, height);
				data.width = width;
				data.height = height;
				

				data.callbackFn(event);

			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyboardPressEvent event(key, 0);
						data.callbackFn(event);
					
						break;
					
					}
					case GLFW_RELEASE:
					{
						KeyboardReleaseEvent event(key);
						data.callbackFn(event);
						break;

					}
					case GLFW_REPEAT:
					{
						KeyboardRepeatEvent event(key, 1);
						data.callbackFn(event);
						break;
					
					}
				}

			});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyboardTypedEvent event(keycode);
				data.callbackFn(event);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						MousePressEvent event(button);
						data.callbackFn(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseReleaseEvent event(button);
						data.callbackFn(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrollEvent event((float)xOffset, (float)yOffset);
				data.callbackFn(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMoveEvent event((float)xPos, (float)yPos);
				data.callbackFn(event);
			});

	}

	void TurboGE::WindowsWindow::shutDown()
	{
		glfwDestroyWindow(m_window);
	}

	void TurboGE::WindowsWindow::onUpdate()
	{
		/* Poll for and process events */
		glfwPollEvents();
		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);


	}

	void TurboGE::WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
	}
}
