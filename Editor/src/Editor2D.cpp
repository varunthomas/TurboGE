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
#include<chrono>

namespace TurboGE
{

    Editor2D::Editor2D()
    {
        m_Renderer = Renderer::Create();
        m_Renderer->Init();
        renderer2DInstance.Init();

        OnAttach();

    }

    void Editor2D::OnAttach()
    {
        m_Scene = std::make_shared<Scene>();
        entityPanel(m_Scene);
#if 0
        m_SquareEntity = m_Scene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

        auto redSquare = m_Scene->CreateEntity("Red Square");
        redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        m_Camera = m_Scene->CreateEntity("Camera");
        m_Camera.AddComponent<CameraComponent>();

        m_SecCamera = m_Scene->CreateEntity("Sec camera");
        auto& sc = m_SecCamera.AddComponent<CameraComponent>();
        sc.primary = false;


        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
                auto& translate = GetComponent<TransformComponent>().translate;
                translate.x = rand() % 10 - 5.0f;
            }

            void OnUpdate(Time ts)
            {
                float speed = 5.0f;
                auto& translate = GetComponent<TransformComponent>().translate;

                if (Input::isKeyPressed(Key::Up))
                {
                    translate.y += speed * ts;
                }
                if (Input::isKeyPressed(Key::Down))
                {
                    translate.y -= speed * ts;
                }
                if (Input::isKeyPressed(Key::Left))
                {
                    translate.x -= speed * ts;
                }
                if (Input::isKeyPressed(Key::Right))
                {
                    translate.x += speed * ts;
                }

            }

            void OnDestroy()
            {
            }
        };

        m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_SecCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif


        FrameBufferSpec fbSpec;
        fbSpec.formats = { FrameBufferFormat::RGBA8, FrameBufferFormat::RED_INT, FrameBufferFormat::DEPTH24_STENCIL8 };
        fbSpec.width = 1280;
        fbSpec.height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
        m_CheckTexture = Texture2D::Create("assets/textures/Checkerboard.png");
        m_SpriteSheet = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

        m_SpriteStairs = SubTexture2D::Create(m_SpriteSheet, { 7,6 }, { 128, 128 }, { 1,1 });
        m_SpriteBarrel = SubTexture2D::Create(m_SpriteSheet, { 8,2 }, { 128, 128 }, { 1,1 });
        m_SpriteTree = SubTexture2D::Create(m_SpriteSheet, { 2,1 }, { 128, 128 }, { 1,2 });
    }

    void Editor2D::onUpdate(Time delta)
    {

        //FOR THIS TO WORK, WE NEED TO CALL INVALIDATE() ON FRAMEBUFFER WHICH IS BASICALLY SETTING UP BUFFER AGAIN WITH NEW WIDTH AND HEIGHT. THIS CAN
        //CAUSE LOT OF PERFORMANCE. SO I COMMENTED THIS OUT AND CALLED OnRESIZE() EVERYTIME EVEN WHEN NO RESIZE HAPPENS SO THAT I DONT HAVE TO CALL OPENGL FRAMEBUFFER
        if (FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            std::cout << "Resize " << spec.width << " " << spec.height << " " << m_ViewportSize.x  << m_ViewportSize.y<< "\n";
            //m_FrameBuffer->SetFrameSpec({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
            //m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        if(m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
            m_EditorCamera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        TGE_PROFILE_FUNCTION();
        renderer2DInstance.ResetStats();
        {
            TGE_PROFILE_SCOPE("Render prep");
            m_FrameBuffer->Bind();
            m_Renderer->setClearColor();
            m_Renderer->Clear();
        }
#if 0
        {
            TGE_PROFILE_SCOPE("Camera Controller");
            if(m_ViewportFocused)
                m_CameraController.OnUpdate(delta);
        }
#endif
        {
            TGE_PROFILE_SCOPE("Draw Render");
#if 0
            renderer2DInstance.StartScene(m_CameraController.GetCamera());
            renderer2DInstance.DrawQuad(glm::vec2(-1.0f, 0.0f), {0.8f, 0.2f, 0.3f, 1.0f}, { 0.8f, 0.8f });
            renderer2DInstance.DrawQuad(glm::vec2(0.5f, -0.5f), { 0.2f, 0.3f, 0.8f, 1.0f }, { 0.5f, 0.75f });
            //renderer2DInstance.DrawQuad(glm::vec3(-5.0f, -5.0f, -0.1f), { 20.0f, 20.0f }, m_CheckTexture, 10.0f);
            //renderer2DInstance.DrawQuad({ 2.0f, 2.0f }, { 10.0f, 10.0f }, m_CheckTexture);

            for (float y = -5.0f; y < 5.0f; y += 0.5f)
            {
                for (float x = -5.0f; x < 5.0f; x += 0.5f)
                {
                    glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                    renderer2DInstance.DrawQuad(glm::vec2(x, y), color, { 0.45f, 0.45f });
                }
            }
            renderer2DInstance.EndScene();
#endif

#if 0
            renderer2DInstance.StartScene(m_CameraController.GetCamera());
            //renderer2DInstance.DrawQuad({ 0.0f, 0.0f, 0.2f}, { 1.0f, 1.0f }, m_SpriteSheet, 1.0f);

            renderer2DInstance.DrawQuad(glm::vec3( 0.0f, 0.0f, 0.2f), { 1.0f, 1.0f }, m_SpriteStairs, 1.0f);
            renderer2DInstance.DrawQuad(glm::vec3(1.0f, 0.0f, 0.2f), { 1.0f, 1.0f }, m_SpriteBarrel, 1.0f);
            renderer2DInstance.DrawQuad(glm::vec3(2.0f, 0.0f, 0.2f), { 1.0f, 2.0f }, m_SpriteTree, 1.0f);
            renderer2DInstance.EndScene();
            
#endif

#if 1

            m_EditorCamera.OnUpdate(delta);

            //m_Scene->onUpdate(delta);
            m_Scene->onUpdateEditor(delta, m_EditorCamera);

            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_BoundsArray[0].x;
            my -= m_BoundsArray[0].y;

            glm::vec2 viewportSize = m_BoundsArray[1] - m_BoundsArray[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;
            //std::cout << "mousex " << mouseX << "mousey " << mouseY << std::endl;
            //std::cout << "viewportx " << viewportSize.x << "viewporty " << viewportSize.y << std::endl;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_FrameBuffer->GetPixelData(1, mouseX, mouseY);
                //std::cout << pixelData << std::endl;
                //HZ_CORE_WARN("Pixel data = {0}", pixelData);
            }

            //glm::vec2 viewPortSize = { m_BoundsArray[1].x - m_BoundsArray[0].x , m_BoundsArray[1].y - m_BoundsArray[0].y };
            //my = viewPortSize.y - my;

            //if (my >= 0 && my <= viewPortSize.y && mx >= 0 && mx <= viewPortSize.x)
            //{
                
                //int pixelData = m_FrameBuffer->GetPixelData(1, (int)mx, (int)my);
                //std::cout << pixelData << std::endl;
            //}


            //std::cout << mx << " " << my << std::endl;
#endif
            m_FrameBuffer->Unbind();
        }
    }

    void Editor2D::onEvent(Event& e)
    {
        //RENDER NEW AREAS AFTER SIZE IS INCREASED
        
        if (e.getEventType() == EventType::WindowSizeEvent)
        {
            std::cout << "Window resized\n";
            auto& winEvent = dynamic_cast<WindowSizeEvent&>(e);
            m_Renderer->setViewPort(winEvent.GetWidth(), winEvent.GetHeight());
        }

        if (e.getEventType() == EventType::MouseScrollEvent)
        {
            m_EditorCamera.OnEvent(e);
        }
#if 0
        if(m_ViewportHovered)
            m_CameraController.onEvent(e);
#endif

        //KEY PRESS

        //SHORTCUTS
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::O))
        {
            LoadScene();
            
        }
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::N))
        {
            NewScene();

        }
        if (Input::isKeyPressed(Key::LeftControl) && Input::isKeyPressed(Key::S))
        {
            SaveScene();

        }

        //GIZMOS
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
        else
        {
            std::cout << "Docking not enabled\n";
        }
        style.WindowMinSize.x = minWinSizeX;
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("New", "ctrl+N"))
                {
                    NewScene();
                }
                if (ImGui::MenuItem("Save as...", "ctrl+S"))
                {
                    SaveScene();
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

            ImGui::EndMenuBar();
        }

        entityPanel.OnImGuiRender();

        uint32_t textureID = m_FrameBuffer->GetID();
        ImGui::Begin("Color settings");
        ImGui::Text("Draw calls: %d", renderer2DInstance.GetStats().drawCalls);
        ImGui::Text("Quad count: %d", renderer2DInstance.GetStats().quadCount);

        ImGui::Separator();
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Editor View");

        //READ PIXEL
        auto viewportMinBound = ImGui::GetWindowContentRegionMin();
        auto viewportMaxBound = ImGui::GetWindowContentRegionMax();
        //auto cursor = ImGui::GetCursorPos(); //THIS IS NOT THE MOUSE CURSOR POSITION


        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); //PIXEL WIDTH AND HEIGHT OF VIEWPORT
        auto viewportOffset = ImGui::GetWindowPos(); //TOPLEFT PIXEL CORDINATE WHERE THE VIEWPORT START
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        //minBound.x += cursor.x;
        //minBound.y += cursor.y;
        //ImVec2 maxBound = { minBound.x + m_ViewportSize.x, minBound.y + m_ViewportSize.y };
        m_BoundsArray[0] = { viewportMinBound.x + viewportOffset.x, viewportMinBound.y + viewportOffset.y };
        m_BoundsArray[1] = { viewportMaxBound.x + viewportOffset.x, viewportMaxBound.y + viewportOffset.y };
        //std::cout << m_ViewportSize.x << " and " << m_ViewportSize.y << std::endl;
        //SOME MORE CHANGES PRESENT IN CHERNO WHICH I DONT THINK IS NEEDED HERE
        ImVec2 aspx = ImGui::GetContentRegionAvail();
        //m_CameraController.onResize(aspx.x, aspx.y);
        ImGui::Image((void*)textureID, aspx, ImVec2{ 0,1 }, ImVec2{ 1,0 });

        //GIZMOS

        auto selectedEntity = entityPanel.GetSelectedEntity();
        if (selectedEntity && m_TransformGizmo != -1)
        {
            ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

            //float windowWidth = (float)ImGui::GetWindowWidth();
            //float windowHeight = (float)ImGui::GetWindowHeight();
            //ImGuizmo::SetRect(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y, windowWidth, windowHeight);
            ImGuizmo::SetRect(m_BoundsArray[0].x, m_BoundsArray[0].y, m_BoundsArray[1].x - m_BoundsArray[0].x, m_BoundsArray[1].y - m_BoundsArray[0].y);

            //auto cameraEntity = m_Scene->GetPrimaryCameraEntity();
            //const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
            //const glm::mat4& cameraProjection = camera.GetProjection();
            //glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>()());
            
            glm::mat4 cameraProjection = m_EditorCamera.GetProjection();
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
            m_Scene = std::make_shared<Scene>();
            entityPanel(m_Scene);
            SceneSerializer deserializer(m_Scene);
            deserializer.Load(*filepath);
        }
    }

    void Editor2D::SaveScene()
    {

        std::optional<std::string> filepath = FileDialogs::SaveFile("Turbo Scene (*.turbo)\0*.turbo\0");
        if (filepath)
        {
            SceneSerializer serializer(m_Scene);
            serializer.Save(*filepath);
        }
    }

    void Editor2D::NewScene()
    {
        m_Scene = std::make_shared<Scene>();
        entityPanel(m_Scene);
    }

    Editor2D* Editor2D::getLayer()
    {
        return this;
    }

    Editor2D::~Editor2D()
    {
        std::cout << "Dlete Editor2D\n";
    }

}