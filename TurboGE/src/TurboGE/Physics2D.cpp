#include"tgepch.h"
#include"Physics2D.h"
#include"TurboGE/Scene/Components.h"
#include"TurboGE/Scene/Scene.h"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include"box2d/b2_body.h"
#include<box2d/b2_world.h>

namespace TurboGE
{
	Physics2D::Physics2D(std::shared_ptr<Scene>& scene)
		//:m_Scene{scene}
	{
		//std::cout << "Physics created\n";
		m_Scene = scene;
		world = new b2World({ 0.0f, -9.8f });

		auto view = m_Scene->m_registry.view<Rigidbody2D>();
		for (auto entity : view)
		{
			//std::cout << "###\n";
			Entity e{ entity, m_Scene.get()};
			auto& rb = view.get<Rigidbody2D>(entity);
			auto& transform = e.GetComponent<TransformComponent>();
			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rb.type;
			bodyDef.position.Set(transform.translate.x, transform.translate.y);
			bodyDef.angle = transform.rotate.z;
			b2Body* body = world->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.fixedRotation);
			rb.body = body;
			if (e.HasComponent<Fixture2D>())
			{
				auto& boxComponent = e.GetComponent<Fixture2D>();
				b2PolygonShape dynamicBox;
				dynamicBox.SetAsBox(transform.scale.x /2, transform.scale.y /2);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &dynamicBox;
				fixtureDef.density = boxComponent.density;
				fixtureDef.friction = boxComponent.friction;
				fixtureDef.restitution = boxComponent.restitution;
				fixtureDef.restitutionThreshold = boxComponent.restitutionThreshold;
				body->CreateFixture(&fixtureDef);

			}
			if (e.HasComponent<CircleFixture2D>())
			{
				auto& circleComponent = e.GetComponent<CircleFixture2D>();
				b2CircleShape dynamicCircle;
				dynamicCircle.m_p.Set(circleComponent.offset.x, circleComponent.offset.y);
				dynamicCircle.m_radius = transform.scale.x * circleComponent.radius;
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &dynamicCircle;
				fixtureDef.density = circleComponent.density;
				fixtureDef.friction = circleComponent.friction;
				fixtureDef.restitution = circleComponent.restitution;
				fixtureDef.restitutionThreshold = circleComponent.restitutionThreshold;
				body->CreateFixture(&fixtureDef);

			}
		}
		//std::cout << m_Scene.use_count() << "\n";
		//std::cout << "--- " << "\n";
	}

	void Physics2D::UpdatePhysics(Time& timeStep, entt::entity entityID)
	{

		world->Step(timeStep, velocityIterations, positionIterations);
		Entity e{ entityID, m_Scene.get() };
		auto& rb = e.GetComponent<Rigidbody2D>();

		b2Body* body = (b2Body*)rb.body;
		b2Vec2 position = body->GetPosition();
		auto& transform = e.GetComponent<TransformComponent>();
		transform.translate.x = position.x;
		transform.translate.y = position.y;
		float angle = body->GetAngle();
		transform.rotate.z = angle;

	}

	Physics2D::~Physics2D()
	{
		//std::cout << "Dest physics\n";
		if (world)
		{
			delete world;
			world = nullptr;
		}
	}
}