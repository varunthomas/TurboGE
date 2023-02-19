#pragma once
#include"VertexArray.h"
#include"Shader.h"
#include"Camera.h"
namespace TurboGE
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual void setClearColor() = 0;
		virtual void Clear() = 0;
		virtual void StartScene(const OrthographicCamera&) = 0;
		virtual void Submit(std::unique_ptr<Shader>&, std::unique_ptr<VertexArray>&) = 0;

		static Renderer* Create();
	};
}