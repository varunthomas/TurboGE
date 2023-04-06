#pragma once
#include"EntityWrapper.h"

namespace TurboGE
{
	class ScriptableEntity
	{
		Entity m_Entity;

	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
		friend class Scene;
	};
}