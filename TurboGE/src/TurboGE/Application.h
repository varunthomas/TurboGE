#pragma once
#include"Core.h"

namespace TurboGE
{
	class TGE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

	};
	Application* CreateApplication();

}

