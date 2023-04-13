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
		std::shared_ptr<Scene> m_Scene;
		
	public:
		SceneSerializer(std::shared_ptr<Scene> scene)
			:m_Scene{ scene } {};
		void Save(const std::string&);
		void Load(const std::string&);
		void SerializeEntities(YAML::Emitter& out, Entity& entity);

		template<typename T>
		void ConstructSave(YAML::Emitter& out, Entity& entity);
		
	};
}