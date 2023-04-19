#include<tgepch.h>
#include"Scene.h"
#include"Components.h"

#include"EntityWrapper.h"
#include <GLFW/glfw3.h>
#include"TurboGE/Input.h"

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

	void Scene::DestroyEntity(entt::entity e)
	{
		m_registry.destroy(e);
	}

	void Scene::onUpdateEditor(Time& t, EditorCamera& camera)
	{
		renderer2DInstance.StartScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		Renderer2D& rendererInstance = Renderer2D::getInstance();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			rendererInstance.DrawQuad<glm::mat4>(transform(), sprite.color, (int)entity);
		}

		renderer2DInstance.EndScene();
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
}
