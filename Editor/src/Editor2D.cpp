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
        /*if (FrameBufferSpec spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            std::cout << "Resize " << spec.width << " " << spec.height << " " << m_ViewportSize.x  << m_ViewportSize.y<< "\n";
            m_FrameBuffer->SetFrameSpec({ (uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y });
            m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }*/

        m_Scene->OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        TGE_PROFILE_FUNCTION();
        renderer2DInstance.ResetStats();
        {
            TGE_PROFILE_SCOPE("Render prep");
            m_FrameBuffer->Bind();
            m_Renderer->setClearColor();
            m_Renderer->Clear();
        }
        {
            TGE_PROFILE_SCOPE("Camera Controller");
            if(m_ViewportFocused)
                m_CameraController.OnUpdate(delta);
        }
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
            //renderer2DInstance.StartScene(m_CameraController.GetCamera());

            m_Scene->onUpdate(delta);
            //renderer2DInstance.EndScene();
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

        if(m_ViewportHovered)
            m_CameraController.onEvent(e);


        //KEY PRESS
        if (Input::isKeyPressed(Key::O) && Input::isKeyPressed(Key::LeftControl))
        {
            LoadScene();
            
        }
        if (Input::isKeyPressed(Key::N) && Input::isKeyPressed(Key::LeftControl))
        {
            NewScene();

        }
        if (Input::isKeyPressed(Key::S) && Input::isKeyPressed(Key::LeftControl))
        {
            SaveScene();

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
        ImGui::Begin("Dockspace");
        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        //SOME MORE CHANGES PRESENT IN CHERNO WHICH I DONT THINK IS NEEDED HERE
        ImVec2 aspx = ImGui::GetContentRegionAvail();
        m_CameraController.onResize(aspx.x, aspx.y);
        ImGui::Image((void*)textureID, aspx, ImVec2{ 0,1 }, ImVec2{ 1,0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void Editor2D::LoadScene()
    {
        std::string filepath = FileDialogs::OpenFile("Turbo Scene (*.turbo)\0*.turbo\0");
        if (!filepath.empty())
        {
            m_Scene = std::make_shared<Scene>();
            entityPanel(m_Scene);
            SceneSerializer deserializer(m_Scene);
            deserializer.Load(filepath);
        }
    }

    void Editor2D::SaveScene()
    {

        std::string filepath = FileDialogs::SaveFile("Turbo Scene (*.turbo)\0*.turbo\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_Scene);
            serializer.Save(filepath);
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