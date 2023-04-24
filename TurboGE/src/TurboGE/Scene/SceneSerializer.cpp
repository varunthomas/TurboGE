#include"tgepch.h"
#include"SceneSerializer.h"
#include"Components.h"
#include<yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace TurboGE
{

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	template<typename T>
	void SceneSerializer::ConstructSave(YAML::Emitter& out, Entity& entity)
	{
		auto component = entity.HasComponent<T>();
		if (component)
		{
			auto name = entity.GetName<T>();
			out << YAML::Key << name.c_str();
			out << YAML::BeginMap;

			if constexpr (std::is_same_v<T, TagComponent>)
			{
				out << YAML::Key << "Tag" << YAML::Value << component->tag;
			}
			else if constexpr (std::is_same_v<T, TransformComponent>)
			{
				out << YAML::Key << "Rotation" << YAML::Value << component->rotate;
				out << YAML::Key << "Translation" << YAML::Value << component->translate;
				out << YAML::Key << "Scale" << YAML::Value << component->scale;
			}
			else if constexpr (std::is_same_v<T, CameraComponent>)
			{
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)component->camera.GetProjectionType();
				out << YAML::Key << "Primary" << YAML::Value << component->primary;
				out << YAML::Key << "Orthographic far clip" << YAML::Value << component->camera.GetOrthographicFarClip();
				out << YAML::Key << "Orthographic near clip" << YAML::Value << component->camera.GetOrthographicNearClip();
				out << YAML::Key << "Orthographic Size" << YAML::Value << component->camera.GetOrthoSize();
				out << YAML::Key << "Perspective FOV" << YAML::Value << component->camera.GetPerspectiveVerticalFOV();
				out << YAML::Key << "Perspective Far clip" << YAML::Value << component->camera.GetPerspectiveFarClip();
				out << YAML::Key << "Perspective near clip" << YAML::Value << component->camera.GetPerspectiveNearClip();
			}
			else if constexpr (std::is_same_v<T, SpriteRendererComponent>)
			{
				out << YAML::Key << "Color" << YAML::Value << component->color;
			}
			out << YAML::EndMap;
		}
	}

	void SceneSerializer::SerializeEntities(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456";

		ConstructSave<TagComponent>(out, entity);
		ConstructSave<TransformComponent>(out, entity);
		ConstructSave<CameraComponent>(out, entity);
		ConstructSave<SpriteRendererComponent>(out, entity);

		out << YAML::EndMap;
	}

	void SceneSerializer::Save(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value <<
			YAML::BeginSeq;

		m_Scene->m_registry.each([&](auto entityID) {
			Entity e = { entityID, m_Scene.get()};
			if (!e)
				return;

			SerializeEntities(out, e);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::Load(const std::string& filePath)
	{

		YAML::Node node = YAML::LoadFile(filePath);
		if (!node["Scene"])
		{
			TURBO_ASSERT("Corrupt file",0);
			return;
		}

		auto entities = node["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				auto tag = entity["TagComponent"]["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntity(tag);

				auto transformComponent = entity["TransformComponent"];

				auto& tc = deserializedEntity.GetComponent<TransformComponent>();
				tc.rotate = transformComponent["Rotation"].as<glm::vec3>();
				tc.scale = transformComponent["Scale"].as<glm::vec3>();
				tc.translate = transformComponent["Translation"].as<glm::vec3>();

				if (entity["CameraComponent"])
				{
					auto cameraComponent = entity["CameraComponent"];
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.camera.SetProjectionType((Projection)cameraComponent["ProjectionType"].as<int>());
					cc.camera.SetOrthographicFarClip(cameraComponent["Orthographic far clip"].as<float>());
					cc.camera.SetOrthographicNearClip(cameraComponent["Orthographic near clip"].as<float>());
					cc.camera.SetOrthoSize(cameraComponent["Orthographic Size"].as<float>());
					cc.camera.SetPerspectiveVerticalFOV(cameraComponent["Perspective FOV"].as<float>());
					cc.camera.SetPerspectiveFarClip(cameraComponent["Perspective Far clip"].as<float>());
					cc.camera.SetPerspectiveNearClip(cameraComponent["Perspective near clip"].as<float>());
				}

				if (entity["SpriteRendererComponent"])
				{
					auto spriteRendererComponent = entity["SpriteRendererComponent"];
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}
	}

}