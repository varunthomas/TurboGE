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

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
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

	YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	template<typename T>
	void SceneSerializer::ConstructSave(YAML::Emitter& out, Entity& entity)
	{
		auto component = entity.HasComponent<T>();
		
		if (component)
		{
			std::cout << "***\n";
			//auto name = entity.GetName<T>();
			out << YAML::Key << component->name;
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
			else if constexpr (std::is_same_v<T, CircleRendererComponent>)
			{
				out << YAML::Key << "Color" << YAML::Value << component->color;
				out << YAML::Key << "Thickness" << YAML::Value << component->thickness;
				out << YAML::Key << "Fade" << YAML::Value << component->fade;
			}
			else if constexpr (std::is_same_v<T, Rigidbody2D>)
			{
				out << YAML::Key << "Body Type" << YAML::Value << (int)component->type;
				out << YAML::Key << "Fixed Rotation" << YAML::Value << component->fixedRotation;
			}
			else if constexpr (std::is_same_v<T, Fixture2D>)
			{
				out << YAML::Key << "Size" << YAML::Value << component->size;
				out << YAML::Key << "Density" << YAML::Value << component->density;
				out << YAML::Key << "Friction" << YAML::Value << component->friction;
				out << YAML::Key << "Restitution" << YAML::Value << component->restitution;
				out << YAML::Key << "Restitution threshold" << YAML::Value << component->restitutionThreshold;
			}
			else if constexpr (std::is_same_v<T, CircleFixture2D>)
			{
				out << YAML::Key << "Offset" << YAML::Value << component->offset;
				out << YAML::Key << "Radius" << YAML::Value << component->radius;
				out << YAML::Key << "Density" << YAML::Value << component->density;
				out << YAML::Key << "Friction" << YAML::Value << component->friction;
				out << YAML::Key << "Restitution" << YAML::Value << component->restitution;
				out << YAML::Key << "Restitution threshold" << YAML::Value << component->restitutionThreshold;
			}
			else if constexpr (std::is_same_v<T, PyScriptComponent>)
			{
				out << YAML::Key << "FileName" << YAML::Value << component->fileName;
				//out << YAML::Key << "Create" << YAML::Value << component->create;
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
		ConstructSave<CircleRendererComponent>(out, entity);
		ConstructSave<Rigidbody2D>(out, entity);
		ConstructSave<Fixture2D>(out, entity);
		ConstructSave<CircleFixture2D>(out, entity);
		ConstructSave<PyScriptComponent>(out, entity);

		out << YAML::EndMap;
	}

	std::string SceneSerializer::Serialize()
	{

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value <<
			YAML::BeginSeq;

		m_Scene->m_registry.each([&](auto entityID) {
			Entity e = { entityID, m_Scene.get() };
			if (!e)
				return;

			SerializeEntities(out, e);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::string tmp = out.c_str();
		return tmp;
	}

	void SceneSerializer::Save(const std::string& filePath)
	{
		std::string out = Serialize();

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::Load(const std::string& filePath, bool isFile)
	{
		YAML::Node node;
		if(isFile)
		{
			try
			{
				node = YAML::LoadFile(filePath);
			}
			catch (YAML::ParserException e)
			{
				TURBO_CORE_ERR("Failed to read file {0} : {1}", filePath, e.what());
			}
		}
		else
		{
			node = YAML::Load(filePath);
		}

		if (!node["Scene"])
		{
			TURBO_CLIENT_ERR("Corrupt file {0}", filePath);
			return;
		}

		auto entities = node["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				auto tag = entity["TagComponent"]["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntity(tag);

				if (entity["TransformComponent"])
				{
					std::cout << "Transform add\n";
					auto transformComponent = entity["TransformComponent"];

					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.rotate = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
					tc.translate = transformComponent["Translation"].as<glm::vec3>();
				}
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

				if (entity["CircleRendererComponent"])
				{
					auto circleRendererComponent = entity["CircleRendererComponent"];
					auto& src = deserializedEntity.AddComponent<CircleRendererComponent>();
					src.color = circleRendererComponent["Color"].as<glm::vec4>();
					src.thickness = circleRendererComponent["Thickness"].as<float>();
					src.fade = circleRendererComponent["Fade"].as<float>();
				}

				if (entity["Rigidbody2D"])
				{
					auto rigidbody2D = entity["Rigidbody2D"];
					auto& src = deserializedEntity.AddComponent<Rigidbody2D>();
					auto type = rigidbody2D["Body Type"].as<int>();
					src.type = (Rigidbody2D::BodyType)type;
					src.fixedRotation = rigidbody2D["Fixed Rotation"].as<bool>();
				}

				if (entity["Fixture2D"])
				{
					auto fixture2D = entity["Fixture2D"];
					auto& src = deserializedEntity.AddComponent<Fixture2D>();
					src.size = fixture2D["Size"].as<glm::vec2>();
					src.density = fixture2D["Density"].as<float>();
					src.friction = fixture2D["Friction"].as<float>();
					src.restitution = fixture2D["Restitution"].as<float>();
					src.restitutionThreshold = fixture2D["Restitution threshold"].as<float>();
				}

				if (entity["CircleFixture2D"])
				{
					auto circleFixture2D = entity["CircleFixture2D"];
					auto& src = deserializedEntity.AddComponent<CircleFixture2D>();
					src.offset = circleFixture2D["Offset"].as<glm::vec2>();
					src.radius = circleFixture2D["Radius"].as<float>();
					src.density = circleFixture2D["Density"].as<float>();
					src.friction = circleFixture2D["Friction"].as<float>();
					src.restitution = circleFixture2D["Restitution"].as<float>();
					src.restitutionThreshold = circleFixture2D["Restitution threshold"].as<float>();
				}
				if (entity["PyScriptComponent"])
				{
					std::cout << "pyscript add\n";
					auto pyScriptComponent = entity["PyScriptComponent"];
					auto& src = deserializedEntity.AddComponent<PyScriptComponent>();
					src.fileName = pyScriptComponent["FileName"].as<std::string>();
					//src.create = pyScriptComponent["Create"].as<bool>();
				}

			}
		}
	}
}