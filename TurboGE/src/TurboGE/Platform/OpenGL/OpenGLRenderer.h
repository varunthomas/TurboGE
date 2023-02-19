#pragma once
#include"TurboGE/Renderer/Renderer.h"
#include<memory>

namespace TurboGE
{
	class OpenGLRenderer : public Renderer
	{
		glm::mat4 m_viewProjectionMatrix;
	public:
		OpenGLRenderer();
		void setClearColor() override;
		void Clear() override;
		void StartScene(const OrthographicCamera&);
		void Submit(std::unique_ptr<Shader>&, std::unique_ptr<VertexArray>&) override;
	};
}