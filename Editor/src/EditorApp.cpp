#include<TurboGE.h>
#include"Editor2D.h"

namespace TurboGE
{
	class Editor : public Application
	{

	public:
		Editor()
		{
			GetLayer(new Editor2D());
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