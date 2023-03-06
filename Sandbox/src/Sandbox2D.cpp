#include"tgepch.h"
#include"Sandbox2D.h"
#include"imgui/imgui.h"
#include"glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include"TurboGE/Platform/OpenGL/OpenGLShader.h"
#include"TurboGE/Events/AppEvent.h"

Sandbox2D::Sandbox2D()
{
	m_Renderer.reset(TurboGE::Renderer::Create());
	m_Renderer->Init();


	m_SquareVA.reset(TurboGE::VertexArray::Create());

	//EVERYTHING IS MADE OF TRIANGLE. SO WE NEED TO CREATE SQUARE USING 2 TRIANGLE. SO WE NEED TO SPECIFY 3+3 VERTEX POSITIONS. BUT 2 VERTICES
	//ARE DUPLICATES. THESE DUPLICATES WILL USE MEMORY. SO IN ORDER TO SAVE MEMORY WE REMOVE THE 2 DUPLICATES AND PROVIDE THE REMAINING 4 VERTICES
	//INDEX BUFFER WILL TAKE CARE OF CREATING SQUARE FROM 2 TRIANGLE WITHOUT USING DUPLICATES
	float sqVertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};
	m_SquareVB.reset(TurboGE::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
	TurboGE::VertexLayout layoutsq;
	layoutsq.m_attribVec = { {0, TurboGE::AttribType::Float3, false }, {1, TurboGE::AttribType::Float2, false} };
	layoutsq.MakeLayout();
	m_SquareVA->SetLayout(layoutsq);
	m_SquareVA->BindVertexBuffer();
	//INDICES FOR SQUARE IS 0 1 2 FOR FIRST TRIANGLE. THEN 2 3 0 FOR SECOND TRIANGLE. INDICES ARE NUMBERED IN ANTICLOCKWISE DIR
	unsigned int indicesSQ[] = { 0, 1, 2, 2, 3, 0 };
	m_SquareIB.reset(TurboGE::IndexBuffer::Create(indicesSQ, sizeof(indicesSQ)));
	m_SquareVA->setIndexBuffer(m_SquareIB);


	m_SquareShader.reset(TurboGE::Shader::Create("assets/shaders/Texture.glsl"));

}

void Sandbox2D::onUpdate(TurboGE::Time delta)
{
	m_Renderer->setClearColor();
	m_Renderer->Clear();

	m_CameraController.OnUpdate(delta);

	m_Renderer->StartScene(m_CameraController.GetCamera());

	m_SquareShader->Bind();
	dynamic_cast<TurboGE::OpenGLShader*>(m_SquareShader.get())->uploadUniformFloat4("u_Color", m_SquareColor);


	m_Renderer->Submit(m_SquareShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
}

void Sandbox2D::onEvent(TurboGE::Event& e)
{
	//RENDER NEW AREAS AFTER SIZE IS INCREASED
	if (e.getEventType() == TurboGE::EventType::WindowSizeEvent)
	{
		auto& winEvent = dynamic_cast<TurboGE::WindowSizeEvent&>(e);
		m_Renderer->setViewPort(winEvent.GetWidth(), winEvent.GetHeight());
	}

	m_CameraController.onEvent(e);
}

void Sandbox2D::renderCustom()
{
	ImGui::Begin("Color settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
