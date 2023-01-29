#pragma once

#ifdef TGE_WINDOWS

//extern TurboGE::Application* TurboGE::CreateApplication();

int main(int argc, char** argv)
{
	auto app = TurboGE::CreateApplication();
	app->Run();
	delete app;
}

#endif