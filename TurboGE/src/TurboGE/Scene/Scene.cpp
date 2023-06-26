#include<tgepch.h>
#include"Scene.h"

#include"Components.h"
#include"EntityWrapper.h"
#include <GLFW/glfw3.h>
#include"TurboGE/Input.h"
#include"TurboGE/Scripting/Scripting.h"

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

	void Scene::onUpdateEditor(Time& t, EditorCamera& camera, bool showCollider)
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
					if (highlightedEntity == (int)entity)
					{
						renderer2DInstance.DrawRect(transform(), glm::vec4(1, 0, 0, 1));
					}
					
					if (showCollider)
					{
						DrawRectVisualizer(entity, transform.translate, transform.scale, transform.rotate);
					}
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

				if (highlightedEntity == (int)entity)
				{
					renderer2DInstance.DrawRect(transform(), glm::vec4(1, 0, 0, 1));
				}

				if (showCollider)
				{
					DrawCircleVisualizer(entity, transform.translate, transform.scale);
				}
			}
		}

		renderer2DInstance.EndScene();
	}

	void Scene::onUpdatePlay(Time& t, std::shared_ptr<Physics2D>& physics, bool showCollider)
	{
		m_registry.view<PyScriptComponent>().each([=](auto entity, auto& psc)
			{
				if (!psc.create)
				{
					if (psc.script.get() == nullptr)
					{	
						if (auto it = PyScriptRepo::scriptMap.find(psc.fileName); it == PyScriptRepo::scriptMap.end())
						{
							psc.script = std::make_shared<PyScript>(psc.fileName);
							PyScriptRepo::scriptMap.emplace(std::make_pair(psc.fileName, psc.script));
						}
						else
						{
							psc.script = it->second;
						}
					}
					psc.script->entity = Entity{ entity, this };
					psc.script->OnCreate();
					psc.create = true;
				}
				psc.script->OnUpdate(t);
				
			});

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
					if (showCollider)
					{
						DrawRectVisualizer(entity, transform.translate, transform.scale, transform.rotate);
					}
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
					if (showCollider)
					{
						DrawCircleVisualizer(entity, transform.translate, transform.scale);
					}
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

	void Scene::DrawRectVisualizer(const entt::entity entity, const glm::vec3& translate, const glm::vec3& sc, const glm::vec3& rotate)
	{
		if (Fixture2D* fixture; fixture = m_registry.try_get<Fixture2D>(entity))
		{
			glm::vec3 tc = { translate.x, translate.y, 0.01f };
			auto scale = sc * glm::vec3(fixture->size * 2.0f, 1.0f);
			glm::mat4 quadTransform = glm::translate(glm::mat4(1.0f), tc) * glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), scale);
			renderer2DInstance.DrawRect(quadTransform, glm::vec4(0, 1, 0, 1));
		}
	}

	void Scene::DrawCircleVisualizer(const entt::entity entity, const glm::vec3& translate, const glm::vec3& sc)
	{
		if (CircleFixture2D* circleFixture; circleFixture = m_registry.try_get<CircleFixture2D>(entity))
		{
			auto tc = translate + glm::vec3(circleFixture->offset, 0.01f);
			auto scale = sc.x * glm::vec3(circleFixture->radius * 2.0f);
			glm::mat4 circleTransform = glm::translate(glm::mat4(1.0f), tc) * glm::scale(glm::mat4(1.0f), scale);
			renderer2DInstance.DrawCircle(circleTransform, glm::vec4(0, 1, 0, 1), 0.03f);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(T& component)
	{
		if constexpr (std::is_same_v<T, CameraComponent>)
		{
			component.camera.SetViewPort((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
		else if constexpr (std::is_same_v<T, PyScriptComponent>)
		{
			if (component.fileName.empty())
				return;

			if (PyScriptRepo::scriptMap.find(component.fileName) == PyScriptRepo::scriptMap.end())
			{
				component.script = std::make_shared<PyScript>(component.fileName);
				PyScriptRepo::scriptMap.emplace(std::make_pair(component.fileName, component.script));
			}

		}
	}

	template void Scene::OnComponentAdded<CameraComponent>(CameraComponent& component);
	template void Scene::OnComponentAdded<SpriteRendererComponent>(SpriteRendererComponent& component);
	template void Scene::OnComponentAdded<CircleRendererComponent>(CircleRendererComponent& component);
	template void Scene::OnComponentAdded<Rigidbody2D>(Rigidbody2D& component);
	template void Scene::OnComponentAdded<Fixture2D>(Fixture2D& component);
	template void Scene::OnComponentAdded<CircleFixture2D>(CircleFixture2D& component);
	template void Scene::OnComponentAdded<PyScriptComponent>(PyScriptComponent& component);

}
