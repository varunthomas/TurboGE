#include"tgepch.h"
#include"Editor2D.h"
#include"imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include"TurboGE/Events/AppEvent.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include"TurboGE/Application.h"
#include"TurboGE/Scene/Components.h"
#include"TurboGE/Scene/SceneSerializer.h"
#include"TurboGE/Utils/PlatformUtils.h"
#include"ImGuizmo.h"
namespace TurboGE
{
    void Editor2D::OnAttach()
    {
        m_Renderer = Renderer::Create();
        m_Renderer->Init();
        renderer2DInstance.Init();

        m_Scene = std::make_shared<Scene>(m_ViewportSize);
        entityPanel(m_Scene);

        FrameBufferSpec fbSpec;
        fbSpec.formats = { FrameBufferFormat::RGBA8, FrameBufferFormat::RED_INT, FrameBufferFormat::DEPTH24_STENCIL8 };
        fbSpec.width = 1920;
        fbSpec.height = 1080;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    }

    void Editor2D::onUpdate(Time delta)
    {
        if (FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            m_FrameBuffer->SetFrameSpec({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        }

        TGE_PROFILE_FUNCTION();
        renderer2DInstance.ResetStats();
        {
            TGE_PROFILE_SCOPE("Render prep");
            m_FrameBuffer->Bind();
            m_Renderer->setClearColor();
            m_Renderer->Clear();
            m_FrameBuffer->ClearEntityAttachment(1, -1);
        }

        {
            TGE_PROFILE_SCOPE("Draw Render");

            if (playPanel.isPlay)
            {
                if (playPanel.toggle)
                {
                    tempData = Serialize(); //SAVE CURRENT CONFIG
                    m_Physics = std::make_shared<Physics2D>(m_Scene);
                }
                m_Scene->onUpdatePlay(delta, m_Physics, m_ShowPhysicsColliders);
            }
            else
            {
                m_EditorCamera.OnUpdate(delta);
                m_Scene->onUpdateEditor(delta, m_EditorCamera, m_ShowPhysicsColliders);
                if (playPanel.toggle)
                {
                    m_Physics.reset();
                    Deserialize(); //LOAD CURRENT CONFIG
                }
            }

            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_BoundsArray[0].x;
            my -= m_BoundsArray[0].y;

            glm::vec2 viewportSize = m_BoundsArray[1] - m_BoundsArray[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_FrameBuffer->GetPixelData(1, mouseX, mouseY);
                m_ClickedEntity = pixelData == -1 ? Entity{} : Entity{ (entt::entity)pixelData, m_Scene.get() };
            }

            m_FrameBuffer->Unbind();
        }
    }

    void Editor2D::onEvent(Event& e)
    {
        //RENDER NEW AREAS AFTER SIZE IS INCREASED
        
        if (e.getEventType() == EventType::WindowSizeEvent)
        {
            auto& winEvent = dynamic_cast<WindowSizeEvent&>(e);
            m_Renderer->setViewPort(winEvent.GetWidth(), winEvent.GetHeight());
        }

        if (e.getEventType() == EventType::MouseScrollEvent)
        {
            m_EditorCamera.OnEvent(e);
        }
        if (e.getEventType() == EventType::MousePressEvent)
        {
            auto& mousePressEvent = dynamic_cast<MousePressEvent&>(e);
            if (mousePressEvent.getMouseButton() == (int)MouseCode::ButtonLeft && m_ViewportHovered && !Input::isKeyPressed(Key::LeftAlt) && !ImGuizmo::IsOver())
            {
                entityPanel.SetSelectedEntity(m_ClickedEntity);
                m_Scene->HighlightEntity((int)m_ClickedEntity);
            }
        }

        //KEY PRESS

        //SHORTCUTS
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::O))
        {
            if (m_DialogDone)
            {
                m_DialogDone = false;
                LoadScene();
            }
        }
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::N))
        {
            NewScene();

        }
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::LeftAlt) && Input::isKeyPressed(Key::S))
        {
            if (m_DialogDone)
            {
                m_DialogDone = false;
                SaveScene();
            }
            

        }
        else if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::S))
        {
            if (m_CurrentSceneFile.empty())
            {
                if (m_DialogDone)
                {
                    m_DialogDone = false;
                    SaveScene();
                }
            }
            else
            {
                SaveScene(m_CurrentSceneFile);
            }
        }

        //GIZMOS

        if (Input::inputViewPort == Input::InputViewport::EDITOR)
        {
            if (Input::isKeyPressed(Key::G))
            {
                m_TransformGizmo = (int)ImGuizmo::OPERATION::TRANSLATE;
            }
            else if (Input::isKeyPressed(Key::R))
            {
                m_TransformGizmo = (int)ImGuizmo::OPERATION::ROTATE;
            }
            else if (Input::isKeyPressed(Key::S)) //TODO::Change S in Zoom
            {
                m_TransformGizmo = (int)ImGuizmo::OPERATION::SCALE;
            }

            //SNAP
            if (Input::isKeyPressed(Key::LeftControl))
            {
                m_Snap = !m_Snap; //TODO:: Disable snap when ctrl+O for opening file
            }
        }

    }

    void Editor2D::renderCustom()
    {
        TGE_PROFILE_FUNCTION();


        bool v = true;
        bool* p_open = &v;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "ctrl+N"))
                {
                    NewScene();
                }
                if (ImGui::MenuItem("Save as...", "ctrl+alt+S"))
                {
                    SaveScene();
                }
                else if (ImGui::MenuItem("Save", "ctrl+S"))
                {
                    if (m_CurrentSceneFile.empty())
                    {
                        SaveScene();
                    }
                    else
                    {
                        SaveScene(m_CurrentSceneFile);
                    }
                }
                if (ImGui::MenuItem("Open..", "ctrl+O"))
                {
                    LoadScene();
                }
                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                {
                    Application::Get().Close();
                    *p_open = false;
                }
                ImGui::EndMenu();
            }

            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 30 * 3); // Adjust the width and spacing according to your needs
            if (ImGui::Button("-", ImVec2(30, 0)))
            {
                Application::Get().Minimize();
            }
            ImGui::SameLine(0, 0);
            if (ImGui::Button("[ ]", ImVec2(30, 0)))
            {
                m_RestoreDown == true ? Application::Get().RestoreDown() : Application::Get().RestoreUp();
                m_RestoreDown = !m_RestoreDown;
            }
            ImGui::SameLine(0, 0);
            if (ImGui::Button("X", ImVec2(30, 0)))
            {
                Application::Get().Close();
            }

            ImGui::EndMenuBar();
        }

        entityPanel.OnImGuiRender();
        browserPanel.OnImGuiRender();

        playPanel.OnImGuiRender();



        uint32_t textureID = m_FrameBuffer->GetID();
        ImGui::Begin("Color settings");
        ImGui::Text("Draw calls: %d", renderer2DInstance.GetStats().drawCalls);
        ImGui::Text("Quad count: %d", renderer2DInstance.GetStats().quadCount);

        ImGui::Separator();
        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Editor View");

        //READ PIXEL
        auto viewportMinBound = ImGui::GetWindowContentRegionMin(); //GET LOWEST VIEWPORT PIXEL X AND Y (0 for x and an offset for y because of the option bar
        auto viewportMaxBound = ImGui::GetWindowContentRegionMax(); //GET HIGHEST VIEWPORT PIXEL OF X AND Y (Y will also include the pixel height of option bar

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        m_ViewportFocused == true ? Input::inputViewPort = Input::InputViewport::EDITOR : Input::inputViewPort = Input::InputViewport::PANEL;

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); //PIXEL WIDTH AND HEIGHT OF VIEWPORT EXCLUDING HEIGHT OF option BAR
        auto viewportOffset = ImGui::GetWindowPos(); //GET OFFSET FROM ACTUAL SCREEN COORD TO VIEWPORT COORD TOP LEFT
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        //BELOW WILL CONVERT VIEWPORT COORD TO FULL SCREEN COORD
        m_BoundsArray[0] = { viewportMinBound.x + viewportOffset.x, viewportMinBound.y + viewportOffset.y };
        m_BoundsArray[1] = { viewportMaxBound.x + viewportOffset.x, viewportMaxBound.y + viewportOffset.y };

        ImGui::Image((void*)textureID, viewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAG_DROP"))
            {
                
                const char* path = (const char*)payload->Data;
                std::string loadFile(path);
                LoadScene(loadFile);
            }
            ImGui::EndDragDropTarget();
        }

        //GIZMOS

        auto selectedEntity = entityPanel.GetSelectedEntity();
        if (selectedEntity && m_TransformGizmo != -1)
        {
            ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_BoundsArray[0].x, m_BoundsArray[0].y, m_BoundsArray[1].x - m_BoundsArray[0].x, m_BoundsArray[1].y - m_BoundsArray[0].y);
            
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetView();

            //ENTITY

            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc();

            //SNAPPING
            if (m_Snap)
            {
                float snapValue = 0.5f;
                if (ImGuizmo::OPERATION::ROTATE == (ImGuizmo::OPERATION)m_TransformGizmo)
                {
                    snapValue = 45.0f;
                }

                float snapValues[3] = { snapValue, snapValue, snapValue };
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_TransformGizmo, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, m_Snap ? snapValues : nullptr);
            }
            else //NO SNAPPING
            {
                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_TransformGizmo, ImGuizmo::LOCAL, glm::value_ptr(transform));
            }

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

                tc.translate = translation;
                tc.rotate = rotation;
                tc.scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void Editor2D::LoadScene()
    {
        std::optional<std::string> filepath = FileDialogs::OpenFile("Turbo Scene (*.turbo)\0*.turbo\0");
        if (filepath)
        {
            m_DialogDone = true;
            LoadScene(*filepath);
        }
    }

    void Editor2D::LoadScene(const std::string& filePath)
    {
        if (filePath == "C")
            return;
        m_CurrentSceneFile = filePath;
        m_Scene = std::make_shared<Scene>(m_ViewportSize);
        entityPanel(m_Scene);
        SceneSerializer deserializer(m_Scene);
        deserializer.Load(filePath);

    }

    void Editor2D::SaveScene()
    {

        std::optional<std::string> filepath = FileDialogs::SaveFile("Turbo Scene (*.turbo)\0*.turbo\0");
        if (filepath)
        {
            m_DialogDone = true;
            SaveScene(*filepath);
        }
    }

    void Editor2D::SaveScene(const std::string& filePath)
    {
        if (filePath == "C")
            return;
        SceneSerializer serializer(m_Scene);
        serializer.Save(filePath);
    }

    std::string Editor2D::Serialize()
    {
        SceneSerializer serializer(m_Scene);
        return serializer.Serialize();
    }

    void Editor2D::Deserialize()
    {
        m_Scene = std::make_shared<Scene>(m_ViewportSize);
        entityPanel(m_Scene);
        SceneSerializer deserializer(m_Scene);
        deserializer.Load(tempData, false);
    }

    void Editor2D::NewScene()
    {
        m_Scene = std::make_shared<Scene>(m_ViewportSize);
        entityPanel(m_Scene);
    }

    Editor2D* Editor2D::getLayer()
    {
        return this;
    }
}