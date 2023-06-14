#include"tgepch.h"
#include"Scripting.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>


namespace TurboGE
{
    PyScript::PyScript(const std::string& scriptName)
    {
        CreateScript(scriptName);

        std::filesystem::path scriptPath = std::filesystem::current_path() / "scripts";
        PyObject* sys = PyImport_ImportModule("sys");
        PyObject* sys_path = PyObject_GetAttrString(sys, "path");
        PyObject* folder_path = PyUnicode_FromString(scriptPath.string().c_str());
        PyList_Append(sys_path, folder_path);

        Py_XDECREF(sys);
        Py_XDECREF(sys_path);
        Py_XDECREF(folder_path);

    }
	void PyScript::Init()
	{
        //std::filesystem::current_path("scripts");
		Py_Initialize();

        /*PyObject* sys = PyImport_ImportModule("sys");
        PyObject* sys_path = PyObject_GetAttrString(sys, "path");
        PyObject* folder_path = PyUnicode_FromString(std::filesystem::current_path().string().c_str());
        PyList_Append(sys_path, folder_path);

        Py_XDECREF(sys);
        Py_XDECREF(sys_path);
        Py_XDECREF(folder_path);*/


        /*
        // Execute the Python script
        FILE* file = fopen("hello.py", "rb");
        if (file)
        {
            PyRun_SimpleFile(file, "hello.py");
            fclose(file);
        }
        else
        {
            
            TURBO_CLIENT_ERR("Failed to open Python script\n");
            Py_Finalize();
            return;
        }

        // Call the hello() function from Python
        PyObject* moduleName = PyUnicode_FromString("hello");
        PyObject* module = PyImport_Import(moduleName);
        Py_XDECREF(moduleName);

        if (module)
        {
            PyObject* helloFunc = PyObject_GetAttrString(module, "hello");
            if (helloFunc && PyCallable_Check(helloFunc))
            {
                PyObject_CallObject(helloFunc, nullptr);
            }
            else
            {
                TURBO_CLIENT_ERR("Failed to call Python function\n");
                Py_XDECREF(helloFunc);
                Py_XDECREF(module);
                Py_Finalize();
                return;
            }
            Py_XDECREF(helloFunc);
            Py_XDECREF(module);
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to import Python module\n");
            Py_Finalize();
            return;
        }

        // Cleanup and shut down the Python interpreter
        
        Py_Finalize();*/

        //std::filesystem::current_path(std::filesystem::current_path().parent_path());
        
	}

    void PyScript::ShutDown()
    {
        Py_Finalize();
    }

    void PyScript::SetPath()
    {
        std::filesystem::current_path("scripts");

        /*PyObject* sys = PyImport_ImportModule("sys");
        PyObject* sys_path = PyObject_GetAttrString(sys, "path");
        PyObject* folder_path = PyUnicode_FromString(std::filesystem::current_path().string().c_str());
        PyList_Append(sys_path, folder_path);

        if (PyList_Check(sys_path)) {
            Py_ssize_t path_length = PyList_Size(sys_path);
            for (Py_ssize_t i = 0; i < path_length; ++i) {
                PyObject* path_item = PyList_GetItem(sys_path, i);
                if (PyUnicode_Check(path_item)) {
                    const char* path_str = PyUnicode_AsUTF8(path_item);
                    printf("%s\n", path_str);
                }
            }
        }

        Py_XDECREF(sys);
        Py_XDECREF(sys_path);
        Py_XDECREF(folder_path);*/
    }

    void PyScript::UnsetPath()
    {
        std::filesystem::current_path(std::filesystem::current_path().parent_path());
    }


    void PyScript::CreateScript(const std::string& scriptName)
    {
        std::cout << "CreateScript\n";
        m_ScriptName = scriptName;
        std::filesystem::path path{ std::filesystem::current_path() };
        path /= "scripts/" + scriptName + ".py";

        std::ofstream ofs(path);
        ofs << "def OnCreate():\n\tprint('Created')\ndef OnUpdate(ts):\n\tprint('Time is ' + str(ts))\n";
        ofs.close();
    }

    void PyScript::OnCreate()
    {
        SetPath();
        std::string pyScript = m_ScriptName + ".py";

        m_File = fopen(pyScript.c_str(), "rb");
        if (m_File)
        {
            PyRun_SimpleFile(m_File, pyScript.c_str());
            //fclose(file);
        }
        else
        {

            TURBO_CLIENT_ERR("Failed to open Python script\n");
            return;
        }

        // Call the hello() function from Python
        //PyObject* moduleName = PyUnicode_FromString(scriptName.c_str());
        m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        m_Module = PyImport_Import(m_ModuleName);
        //Py_XDECREF(m_ModuleName);

        if (m_Module)
        {
            PyObject* OnCreateFunc = PyObject_GetAttrString(m_Module, "OnCreate");
            if (OnCreateFunc && PyCallable_Check(OnCreateFunc))
            {
                PyObject_CallObject(OnCreateFunc, nullptr);
            }
            else
            {
                TURBO_CLIENT_ERR("Failed to call Python function\n");
                Py_XDECREF(OnCreateFunc);
                //Py_XDECREF(m_Module);
                return;
            }
            Py_XDECREF(OnCreateFunc);
            //Py_XDECREF(m_Module);
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to import Python module\n");
            return;
        }

        UnsetPath();

    }

    void PyScript::OnUpdate(float ts)
    {
        SetPath();

        std::string pyScript = m_ScriptName + ".py";

        //FILE* file = fopen(pyScript.c_str(), "rb");
        //if (file)
        //{
            PyRun_SimpleFile(m_File, pyScript.c_str());
          //  fclose(file);
        //}
        //else
        //{

          //  TURBO_CLIENT_ERR("Failed to open Python script\n");
           // return;
        //}

        // Call the hello() function from Python
        //PyObject* moduleName = PyUnicode_FromString(scriptName.c_str());
        //m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        //m_Module = PyImport_Import(m_ModuleName);
        //Py_XDECREF(m_ModuleName);

        if (m_Module)
        {
            PyObject* OnUpdateFunc = PyObject_GetAttrString(m_Module, "OnUpdate");
            if (OnUpdateFunc && PyCallable_Check(OnUpdateFunc))
            {
                    /* Time to call the callback */
                PyObject* arglist = Py_BuildValue("(f)", ts);
                PyObject* result = PyObject_CallObject(OnUpdateFunc, arglist);
                if (result == NULL)
                {
                    if (PyErr_Occurred())
                        PyErr_Print();
                    Py_DECREF(result);
                    Py_XDECREF(arglist);
                    return;
                }
                Py_DECREF(result);
                Py_XDECREF(arglist);
            }
            else
            {
                TURBO_CLIENT_ERR("Failed to call Python function\n");
                Py_XDECREF(OnUpdateFunc);
                //Py_XDECREF(m_Module);
                return;
            }
            Py_XDECREF(OnUpdateFunc);
            //Py_XDECREF(m_Module);
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to import Python module\n");
            return;
        }

        UnsetPath();

    }
	
    PyScript::~PyScript()
    {
        std::cout << "Destroyed\n";
        fclose(m_File);
        Py_XDECREF(m_ModuleName);
        Py_XDECREF(m_Module);
    }
}