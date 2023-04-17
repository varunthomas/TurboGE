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
		/*
		glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//float radius = 10.0f;
		//float camX = static_cast<float>(sin(glfwGetTime()) * radius);
		//float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		//glm::mat4 view;
		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			//glm::vec3(0.0f, 0.0f, 0.0f),
			//glm::vec3(0.0f, 1.0f, 0.0f));

		static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const float cameraSpeed = 2.0f * t; // adjust accordingly

		static bool firstMouse = true;
		static float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		static float pitch = 0.0f;
		static float lastX = 800.0f / 2.0;
		static float lastY = 600.0 / 2.0;
		static float fov = 45.0f;

		//MOUSE
		
		if (Input::isKeyPressed(Key::LeftAlt))
		{
			//MOVE
			float xpos = Input::GetMouseX();
			float ypos = Input::GetMouseY();

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.1f; // change this value to your liking
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (zoomed)
			{
				fov -= yoffsetScroll;
				if (fov < 1.0f)
					fov = 1.0f;
				if (fov > 45.0f)
					fov = 45.0f;
				std::cout << "Camera zooming yoffsetScrool is " << yoffset << "\n";
				zoomed = false;
			}
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);

			//SCROLL
			
		}
		//std::cout << front.x << " " << front.y << " " << front.z << std::endl;



		//KEY
		if (Input::isKeyPressed(Key::W))
			cameraPos += cameraSpeed * cameraFront;
		if (Input::isKeyPressed(Key::S))
			cameraPos -= cameraSpeed * cameraFront;
		if (Input::isKeyPressed(Key::A))
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (Input::isKeyPressed(Key::D))
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);
		glm::mat4 viewproj = projection * view;


		*/
		renderer2DInstance.StartScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		Renderer2D& rendererInstance = Renderer2D::getInstance();
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			rendererInstance.DrawQuad<glm::mat4>(transform(), sprite.color);
		}

		renderer2DInstance.EndScene();
	}

	/*void Scene::onUpdate(Time& t)
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
			renderer2DInstance.StartScene(*mainCamera, cameraTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			Renderer2D& rendererInstance = Renderer2D::getInstance();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				rendererInstance.DrawQuad<glm::mat4>(transform(), sprite.color);
			}

			renderer2DInstance.EndScene();
		}
	}*/

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

	Scene::~Scene()
	{
		std::cout << "Deleted SCENE\n";
	}
}
