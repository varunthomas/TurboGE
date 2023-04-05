#pragma once
#include<glm/glm.hpp>
#include"TurboGE/Renderer/Camera.h"

namespace TurboGE
{
	struct TransformComponent
	{
		glm::mat4 transform{ 1.0f };
		TransformComponent() = default;
		TransformComponent(const glm::mat4& t)
			:transform{ t } {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& c)
			: color(c) {}
	};

	struct TagComponent
	{
		std::string tag = "Untagged Entity";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string_view str)
			: tag(str) {}
	};

	struct CameraComponent
	{
		GameCamera camera;
		bool primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		//CameraComponent(const glm::mat4& projection)
			//: camera(projection) {}
	};
}