#pragma once
#include<memory>
#include"TurboGE/Renderer/Shader.h"
#include"TurboGE/Renderer/Buffer.h"
#include"TurboGE/Renderer/VertexArray.h"
#include"TurboGE/Renderer/Renderer.h"
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Renderer/Texture.h"
#include"Input.h"
#include"Time.h"

//TEMPORARY UNTIL I UNDERSTAND WHY LAYERS ARE USED
class Example
{
	std::unique_ptr<TurboGE::Input> m_Input;
	glm::vec3 m_CameraPos = { 0.5f, 0.5f, 0.0f };
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	float m_CameraRot = 0.0f;

	std::unique_ptr<TurboGE::VertexArray> m_VertexArray;
	std::unique_ptr<TurboGE::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<TurboGE::IndexBuffer> m_IndexBuffer;
	std::unique_ptr<TurboGE::Shader> m_Shader;
	std::unique_ptr<TurboGE::Shader> m_TextureShader;
	std::unique_ptr<TurboGE::Texture2D> m_Texture;

	std::unique_ptr<TurboGE::VertexArray> m_SquareVA;
	std::unique_ptr<TurboGE::VertexBuffer> m_SquareVB;
	std::shared_ptr<TurboGE::IndexBuffer> m_SquareIB;
	std::unique_ptr<TurboGE::Shader> m_SquareShader;

	std::unique_ptr<TurboGE::Renderer> m_Renderer;

	TurboGE::OrthographicCamera m_Camera{ -1.6f, 1.6f, -0.9f, 0.9f };

public:
	Example();
	void onUpdate(Time);
	void renderCustom();
	
};