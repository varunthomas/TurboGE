#pragma once
#include"TurboGE/Scene/EntityWrapper.h"

typedef struct _object PyObject;

namespace TurboGE
{
	class PyScript
	{
		std::string m_ScriptName;
		PyObject* m_ModuleName{};
		PyObject* m_Module{};
		FILE* m_File{};

		void CreateScript(const std::string&);

		public:
			Entity entity;
			PyScript(const std::string&);
			~PyScript();
			static void Init();
			
			void OnCreate();
			void OnUpdate(float);

			static void ShutDown();
	};

	struct PyScriptRepo
	{
		inline static std::unordered_map<std::string, std::shared_ptr<PyScript>> scriptMap;
	};
}