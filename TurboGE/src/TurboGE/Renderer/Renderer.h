#pragma once
#include"VertexArray.h"
namespace TurboGE
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual void setClearColor() = 0;
		virtual void Clear() = 0;
		virtual void Submit(std::unique_ptr<VertexArray>&) = 0;

		static Renderer* Create();
	};
}