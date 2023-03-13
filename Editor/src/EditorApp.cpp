#include<TurboGE.h>


namespace TurboGE
{
	class Editor : public Application
	{

	public:
		Editor()
		{

		}

		~Editor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}