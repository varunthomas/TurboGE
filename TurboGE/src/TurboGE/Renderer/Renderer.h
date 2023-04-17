#pragma once
#include"VertexArray.h"
#include"Shader.h"
#include"Camera.h"
namespace TurboGE
{
	class Renderer
	{
	public:
		virtual ~Renderer() { std::cout << "Deleted renderer\n"; }
		virtual void setClearColor() = 0;
		virtual void Clear() = 0;
		virtual void Init() = 0;
		virtual void setViewPort(uint32_t, uint32_t) = 0;
		virtual void StartScene(const Camera&) = 0;
		virtual void Submit(std::unique_ptr<Shader>&, std::unique_ptr<VertexArray>&, glm::mat4& ) = 0;

		static std::unique_ptr<Renderer> Create();
	};
}