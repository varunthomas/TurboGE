#pragma once

#ifdef TGE_WINDOWS

//extern TurboGE::Application* TurboGE::CreateApplication();

int main(int argc, char** argv)
{
	TurboGE::Logger::Init();
	TURBO_CORE_ERR("This is core log");
	TURBO_CLIENT_ERR("This is client log");
	auto app = TurboGE::CreateApplication();
	app->Run();
	delete app;
}

#endif