#pragma once

typedef struct _object PyObject;

namespace TurboGE
{
	class PyScript
	{
		std::string m_ScriptName;
		PyObject* m_ModuleName;
		PyObject* m_Module;
		FILE* m_File;

		void SetPath();
		void UnsetPath();
		void CreateScript(const std::string&);

		public:
			PyScript(const std::string&);
			~PyScript();
			static void Init();
			
			void OnCreate();
			void OnUpdate(float);

			static void ShutDown();
	};
}