#include"tgepch.h"
#include "ImguiLayer.h"
#include "imgui.h"
#include "TurboGE/Platform/OpenGL/ImguiOpenGLRenderer.h"
#include"TurboGE/Application.h"
#include<glad/glad.h>

namespace TurboGE
{
	ImguiLayer::ImguiLayer()
	{
        TURBO_CORE_ERR("Constructed\n");
	}


	void ImguiLayer::onUpdate()
	{
		static bool show_demo_window = true;
		ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiLayer::onAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImguiLayer::onEvent(Event& e)
	{
        //TURBO_CORE_ERR("Iterating");
        if (e.getEventType() == EventType::MousePressEvent)
        {
            //TURBO_CORE_ERR("Iterating1");
            onMousePressEvent(dynamic_cast<MousePressEvent&>(e));
        }
        else if (e.getEventType() == EventType::MouseReleaseEvent)
        {
            //TURBO_CORE_ERR("Iterating2");
            onMouseReleaseEvent(dynamic_cast<MouseReleaseEvent&>(e));
        }
        else if (e.getEventType() == EventType::MouseMoveEvent)
        {
            //TURBO_CORE_ERR("Iterating3");
            onMouseMoveEvent(dynamic_cast<MouseMoveEvent&>(e));
        }
        else if (e.getEventType() == EventType::MouseScrollEvent)
        {
            //TURBO_CORE_ERR("Iterating4");
            onMouseScrollEvent(dynamic_cast<MouseScrollEvent&>(e));
        }
        else if (e.getEventType() == EventType::KeyboardPressEvent)
        {
            //TURBO_CORE_ERR("Iterating5");
            onKeyboardPressEvent(dynamic_cast<KeyboardPressEvent&>(e));
        }
        else if (e.getEventType() == EventType::KeyboardReleaseEvent)
        {
            //TURBO_CORE_ERR("Iterating6");
            onKeyboardReleaseEvent(dynamic_cast<KeyboardReleaseEvent&>(e));
        }
        else if (e.getEventType() == EventType::KeyboardTypedEvent)
        {
            //TURBO_CORE_ERR("Iterating7");
            onKeyboardTypedEvent(dynamic_cast<KeyboardTypedEvent&>(e));
        }
        else if (e.getEventType() == EventType::WindowSizeEvent)
        {
            //TURBO_CORE_ERR("Iterating8");
            OnWindowResizeEvent(dynamic_cast<WindowSizeEvent&>(e));
        }
	}

	void ImguiLayer::onMousePressEvent(MousePressEvent& e)
	{
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = true;
        //TURBO_CORE_ERR("PRESSED");
	}
    void ImguiLayer::onMouseReleaseEvent(MouseReleaseEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = false;
        //TURBO_CORE_ERR("RELEASE");
    }
    void ImguiLayer::onMouseMoveEvent(MouseMoveEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.getX(), e.getY());
        //TURBO_CORE_ERR("MOVED");
    }

    void ImguiLayer::onMouseScrollEvent(MouseScrollEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MouseWheelH += e.getXOffset();
        io.MouseWheel += e.getYOffset();
        //TURBO_CORE_ERR("SCROLLED");
    }

    void ImguiLayer::onKeyboardPressEvent(KeyboardPressEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.getKey()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    void ImguiLayer::onKeyboardReleaseEvent(KeyboardReleaseEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.getKey()] = false;
    }

    void ImguiLayer::onKeyboardTypedEvent(KeyboardTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        unsigned int c = e.getKey();
        io.AddInputCharacter(c);
    }

    void ImguiLayer::OnWindowResizeEvent(WindowSizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
        //TURBO_CORE_ERR("SIZE");

    }

    ImguiLayer::~ImguiLayer()
    {
        TURBO_CORE_ERR("Destroyed\n");
    }

    
}