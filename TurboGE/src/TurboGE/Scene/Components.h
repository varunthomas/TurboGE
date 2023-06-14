#pragma once
#include<glm/glm.hpp>
#include"TurboGE/Renderer/Camera.h"
#include"TurboGE/Renderer/Texture.h"
#include"ScriptableEntity.h"
#include"TurboGE/Scripting/Scripting.h"
#include<glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>

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
			glm::mat4 rotation = glm::toMat4(glm::quat(rotate));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translate) * rotation * glm::scale(glm::mat4(1.0f), scale);
			
			return transform;
		}
	};

	struct SpriteRendererComponent
	{
		std::string name = "SpriteRendererComponent";

		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		std::shared_ptr<Texture2D> texture;

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

	};

	struct CircleRendererComponent
	{
		std::string name = "CircleRendererComponent";
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct Rigidbody2D
	{
		std::string name = "Rigidbody2D";

		enum class BodyType {Static = 0, Kinematic, Dynamic };
		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* body = nullptr;

		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D&) = default;

	};

	struct Fixture2D
	{
		std::string name = "Fixture2D";
		glm::vec2 size = { 0.5f, 0.5f };
		float density = 1.0f;
		float friction = 0.3f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		Fixture2D() = default;
		Fixture2D(const Fixture2D&) = default;

	};

	struct CircleFixture2D
	{
		std::string name = "CircleFixture2D";
		glm::vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;
		float density = 1.0f;
		float friction = 0.3f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		CircleFixture2D() = default;
		CircleFixture2D(const CircleFixture2D&) = default;

	};

	struct PyScriptComponent
	{
		std::string name = "PyScriptComponent";
		std::string fileName;
		bool create = false;
		std::unique_ptr<PyScript> script;

		PyScriptComponent() = default;
		PyScriptComponent(const PyScriptComponent&) = default;
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