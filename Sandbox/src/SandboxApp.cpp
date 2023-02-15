#include<TurboGE.h>

class Sandbox : public TurboGE::Application
{
public:
	Sandbox()
	{
		//TurboGE::ImguiLayer* layer{ new TurboGE::ImguiLayer() };
		
		//layer->onAttach();
		//setLayerInstance(layer);
	}

	~Sandbox()
	{
	}
};

TurboGE::Application* TurboGE::CreateApplication()
{
	return new Sandbox();
}