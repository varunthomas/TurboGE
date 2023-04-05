#include<tgepch.h>
#include"Scene.h"
#include"Components.h"

#include"EntityWrapper.h"

namespace TurboGE
{
	Entity Scene::CreateEntity(std::string_view tag)
	{
		Entity e{ m_registry.create(), this };
		e.AddComponent<TransformComponent>();
		auto& tagComponent = e.AddComponent<TagComponent>();
		tagComponent.tag = tag.empty() ? "Entity" : tag;
		return e;
	}

	void Scene::onUpdate(Time& t)
	{
		GameCamera* mainCamera{};
		glm::mat4* cameraTransform;
		{
			auto group = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = &transform.transform;
				}
			}
			
		}

		if (mainCamera)
		{
			renderer2DInstance.StartScene(*mainCamera, *cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			Renderer2D& rendererInstance = Renderer2D::getInstance(); //MIGHT GET COSTLY
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				rendererInstance.DrawQuad<glm::mat4>(transform, sprite.color);
			}

			renderer2DInstance.EndScene();
		}
	}

	void Scene::OnResize(uint32_t width, uint32_t height)
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			camera.camera.SetViewPort(width, height);
		}
	}

	Scene::~Scene()
	{
		std::cout << "Deleted SCENE\n";
	}
}
