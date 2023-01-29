#include<TurboGE.h>

class Sandbox : public TurboGE::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};

TurboGE::Application* TurboGE::CreateApplication()
{
	return new Sandbox();
}