#pragma once
#include"TurboGE/Renderer/Renderer.h"

namespace TurboGE
{
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();
		void setClearColor() override;
		void Clear() override;
		void Submit(std::unique_ptr<VertexArray>&) override;
	};
}