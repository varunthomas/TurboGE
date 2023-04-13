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

	 void SceneSerializer::SerializeEntities(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123456";
		
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "Tag" << YAML::Value << entity.GetComponent<TagComponent>().tag;
		out << YAML::EndMap;

		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "Rotation" << YAML::Value << entity.GetComponent<TransformComponent>().rotate;
		out << YAML::Key << "Translation" << YAML::Value << entity.GetComponent<TransformComponent>().translate;
		out << YAML::Key << "Scale" << YAML::Value << entity.GetComponent<TransformComponent>().scale;
		out << YAML::EndMap;

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)entity.GetComponent<CameraComponent>().camera.GetProjectionType();
			out << YAML::Key << "Primary" << YAML::Value << entity.GetComponent<CameraComponent>().primary;
			out << YAML::Key << "Orthographic far clip" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetOrthographicFarClip();
			out << YAML::Key << "Orthographic near clip" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetOrthographicNearClip();
			out << YAML::Key << "Orthographic Size" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetOrthoSize();
			out << YAML::Key << "Perspective FOV" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "Perspective Far clip" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetPerspectiveFarClip();
			out << YAML::Key << "Perspective near clip" << YAML::Value << entity.GetComponent<CameraComponent>().camera.GetPerspectiveNearClip();
			out << YAML::EndMap;
		}
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "Sprite Renderer";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << entity.GetComponent<SpriteRendererComponent>().color;
			out << YAML::EndMap;
		}

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

		const std::string save_dir = "saves";
		std::filesystem::path fp{ save_dir };
		std::filesystem::create_directory(fp);
		std::ofstream fout(save_dir + "/" + filePath);
		fout << out.c_str();
	}

	void SceneSerializer::Load(const std::string& filePath)
	{
		std::filesystem::path fp{ "saves" };
		if (!std::filesystem::exists(fp))
		{
			std::cout << "No Saves found\n";
			return;
		}



		std::ifstream in("saves/" + filePath);
		std::stringstream buffer;
		buffer << in.rdbuf();
		std::string content(buffer.str());

		YAML::Node node = YAML::Load(content);
		if (!node["Scene"])
		{
			std::cout << "Corrupt file\n";
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
					std::cout << "Tag " << tag << std::endl;
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

				if (entity["Sprite Renderer"])
				{
					auto spriteRendererComponent = entity["Sprite Renderer"];
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
				}
			}
		}
	}

}