#pragma once
#include"Scene.h"
#include"EntityWrapper.h"

namespace YAML
{
	class Emitter;
}

namespace TurboGE
{
	
	class SceneSerializer
	{
		const std::unique_ptr<Scene>& m_Scene;
		
	public:
		SceneSerializer(const std::unique_ptr<Scene>& scene)
			:m_Scene{ scene } {};
		void Save(const std::string&);
		void Load(const std::string&, bool isFile = true);
		void SerializeEntities(YAML::Emitter& out, Entity& entity);
		std::string Serialize();
		void Deserialize();

		template<typename T>
		void ConstructSave(YAML::Emitter& out, Entity& entity);
		
	};
}