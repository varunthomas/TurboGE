#pragma once
#include<glm/glm.hpp>
#include"TurboGE/Renderer/Camera.h"
#include"ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TurboGE
{
	struct TransformComponent
	{
		std::string name = "TransformComponent";

		glm::vec3 translate{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotate{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const glm::vec3& t)
			:translate{ t } {}

		glm::mat4 operator()() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotate.x, { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), rotate.y, { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), rotate.z, { 0,0,1 });
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translate) * rotation * glm::scale(glm::mat4(1.0f), scale);
			return transform;
		}
	};

	struct SpriteRendererComponent
	{
		std::string name = "SpriteRendererComponent";

		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& c)
			: color(c) {}
	};

	struct TagComponent
	{
		std::string name = "TagComponent";

		std::string tag = "Untagged Entity";

		void serialize(std::function<void(const std::string& key, std::string value)> Fn)
		{

		}

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string_view str)
			: tag(str) {}
	};

	struct CameraComponent
	{
		std::string name = "CameraComponent";

		GameCamera camera;
		bool primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		//CameraComponent(const glm::mat4& projection)
			//: camera(projection) {}

	};

	template<typename T>
	concept FunctionExists = requires(T a)
	{
		a.OnDestroy();
	};

	struct NativeScriptComponent
	{
		std::string name = "NativeScriptComponent";

		ScriptableEntity* scriptableEntity = {};
		std::function<void()> OnCreateFunction;
		std::function<void(Time)> OnUpdateFunction;
		std::function<void()> OnDestroyFunction;

		void CreateInstance()
		{
			scriptableEntity = new ScriptableEntity();
		}

		template<typename T>
		void Bind()
		{
			OnCreateFunction = [this]()
			{
				((T*)scriptableEntity)->OnCreate();
			};

			OnUpdateFunction = [this](Time t)
			{
				((T*)scriptableEntity)->OnUpdate(t);
			};

			if constexpr (FunctionExists<T>)
			{
				OnDestroyFunction = [this]()
				{
					((T*)scriptableEntity)->OnDestroy();
				};
			}
			else
			{
				OnDestroyFunction = [](){};
			}
		}
	};
}