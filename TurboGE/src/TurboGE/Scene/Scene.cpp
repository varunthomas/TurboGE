#include<tgepch.h>
#include"Scene.h"
#include"Components.h"
#include"TurboGE/Renderer/Renderer2D.h"
#include"EntityWrapper.h"

namespace TurboGE
{
	Entity Scene::CreateEntity(std::string_view tag)
	{
		Entity e{ m_registry.create(), this };
		auto& tagComponent = e.AddComponent<TagComponent>();
		tagComponent.tag = tag.empty() ? "Entity" : tag;
		return e;
	}

	void Scene::onUpdate(Time& t)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		Renderer2D& rendererInstance = Renderer2D::getInstance(); //MIGHT GET COSTLY
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			rendererInstance.DrawQuad<glm::mat4>(transform, sprite.color);
		}
	}

	Scene::~Scene()
	{
		std::cout << "Deleted SCENE\n";
	}
}
