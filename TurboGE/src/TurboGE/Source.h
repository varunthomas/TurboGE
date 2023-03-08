#pragma once
#include<memory>
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Renderer.h"
#include"TurboGE/Renderer/Texture.h"
#include"Time.h"
#include"Events/Event.h"
#include"CameraController.h"

//TEMPORARY UNTIL I UNDERSTAND WHY LAYERS ARE USED
class Example
{

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

	std::unique_ptr<TurboGE::VertexArray> m_VertexArray;
	std::unique_ptr<TurboGE::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<TurboGE::IndexBuffer> m_IndexBuffer;
	std::unique_ptr<TurboGE::Shader> m_Shader;
	std::unique_ptr<TurboGE::Shader> m_TextureShader;
	std::unique_ptr<TurboGE::Texture2D> m_Texture;
	std::unique_ptr<TurboGE::Texture2D> m_CheckTexture;

	std::unique_ptr<TurboGE::VertexArray> m_SquareVA;
	std::unique_ptr<TurboGE::VertexBuffer> m_SquareVB;
	std::shared_ptr<TurboGE::IndexBuffer> m_SquareIB;
	std::unique_ptr<TurboGE::Shader> m_SquareShader;

	std::unique_ptr<TurboGE::Renderer> m_Renderer;

	TurboGE::OrthographicCameraController m_CameraController{ 1280.0f / 720.0f };

public:
	Example();
	void onUpdate(TurboGE::Time);
	void renderCustom();
	void onEvent(TurboGE::Event&);
	
};