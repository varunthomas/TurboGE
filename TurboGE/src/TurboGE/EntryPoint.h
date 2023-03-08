#pragma once

#ifdef TGE_WINDOWS

//extern TurboGE::Application* TurboGE::CreateApplication();

int main(int argc, char** argv)
{
	TurboGE::Logger::Init();
	TGE_BEGIN_PROFILING("Startup", "TGE_startup_profiling.json");
	auto app = TurboGE::CreateApplication();
	TGE_END_PROFILING();

	TGE_BEGIN_PROFILING("Run", "TGE_run_profiling.json");
	app->Run();
	TGE_END_PROFILING();

	TGE_BEGIN_PROFILING("Shutdown", "TGE_shutdown_profiling.json");
	delete app;
	TGE_END_PROFILING();
}

#endif