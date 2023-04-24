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
		~OpenGLRenderer() = default;
		void setClearColor() override;
		void setViewPort(uint32_t, uint32_t) override;
		void Clear() override;
		void Init() override;
		void StartScene(const Camera&);
		void Submit(std::unique_ptr<Shader>&, std::unique_ptr<VertexArray>&, glm::mat4&) override;
	};
}