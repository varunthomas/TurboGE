#include<tgepch.h>
#include"Scene.h"
#include"Components.h"

#include"EntityWrapper.h"
#include <GLFW/glfw3.h>
#include"TurboGE/Input.h"

namespace TurboGE
{
	Scene::Scene(glm::vec2& viewPortSize)
	{
		m_ViewportSize = viewPortSize;
	}

	Entity Scene::CreateEntity(std::string_view tag)
	{
		Entity e{ m_registry.create(), this };
		e.AddComponent<TransformComponent>();
		auto& tagComponent = e.AddComponent<TagComponent>();
		tagComponent.tag = tag.empty() ? "Entity" : tag;
		return e;
	}

	void Scene::DestroyEntity(entt::entity e)
	{
		m_registry.destroy(e);
	}

	void Scene::onUpdateEditor(Time& t, EditorCamera& camera)
	{
		renderer2DInstance.StartScene(camera);
		Renderer2D& rendererInstance = Renderer2D::getInstance();

		{
			auto view = m_registry.view<TransformComponent, SpriteRendererComponent>();
			
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				if (sprite.texture == nullptr)
				{
					rendererInstance.DrawQuad<glm::mat4>(transform(), sprite.color, (int)entity);
				}
				else
				{
					rendererInstance.DrawQuad<glm::mat4, std::shared_ptr<Texture2D>>(transform(), { transform.scale.x, transform.scale.y }, sprite.texture, 1.0f, (int)entity);
				}
			}
		}
		// Draw circles
		{
			auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				rendererInstance.DrawCircle(transform(), circle.color, circle.thickness, circle.fade, (int)entity);
			}
		}

		renderer2DInstance.EndScene();
	}

	void Scene::onUpdatePlay(Time& t, std::shared_ptr<Physics2D>& physics)
	{
		m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.scriptableEntity)
				{
					nsc.CreateInstance();
					nsc.scriptableEntity->m_Entity = Entity{ entity, this };
					nsc.OnCreateFunction();
				}
				nsc.OnUpdateFunction(t);
				nsc.OnDestroyFunction();
			});
		GameCamera* mainCamera{};
		glm::mat4 cameraTransform;
		{
			auto group = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = transform();
				}
			}

		}
		if (mainCamera)
		{

			//PHYSICS
			auto view = m_registry.view<Rigidbody2D>();
			for (auto e : view)
			{
				physics->UpdatePhysics(t,e);
			}

			renderer2DInstance.StartScene(*mainCamera, cameraTransform);



			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			Renderer2D& rendererInstance = Renderer2D::getInstance();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				if (sprite.texture == nullptr)
				{
					rendererInstance.DrawQuad<glm::mat4>(transform(), sprite.color, (int)entity);
				}
				else
				{
					rendererInstance.DrawQuad<glm::mat4, std::shared_ptr<Texture2D>>(transform(), { transform.scale.x, transform.scale.y }, sprite.texture, 1.0f, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					rendererInstance.DrawCircle(transform(), circle.color, circle.thickness, circle.fade, (int)entity);
				}
			}


			renderer2DInstance.EndScene();
		}
	}

	void Scene::OnResize(uint32_t width, uint32_t height)
	{
		m_ViewportSize.x = width;
		m_ViewportSize.y = height;
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			camera.camera.SetViewPort(width, height);
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary == true)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(T& component)
	{
		if constexpr (std::is_same_v<T, CameraComponent>)
		{
			component.camera.SetViewPort((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}

	template void Scene::OnComponentAdded<CameraComponent>(CameraComponent& component);
	template void Scene::OnComponentAdded<SpriteRendererComponent>(SpriteRendererComponent& component);
	template void Scene::OnComponentAdded<CircleRendererComponent>(CircleRendererComponent& component);
	template void Scene::OnComponentAdded<Rigidbody2D>(Rigidbody2D& component);
	template void Scene::OnComponentAdded<Fixture2D>(Fixture2D& component);
	template void Scene::OnComponentAdded<CircleFixture2D>(CircleFixture2D& component);
}
