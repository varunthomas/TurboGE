#pragma once
#include"TurboGE/Time.h"
#include<entt.hpp>
class b2World;
namespace TurboGE
{
	class Scene;

	class Physics2D
	{
		std::shared_ptr<Scene> m_Scene;
		b2World* world = nullptr;
		int32_t velocityIterations = 6;
		int32_t positionIterations = 2;
	public:
		Physics2D(std::shared_ptr<Scene>&);
		~Physics2D();

		void UpdatePhysics(Time&, entt::entity);
		static void ApplyLinearImpulse(void*, float, float);
	};
}