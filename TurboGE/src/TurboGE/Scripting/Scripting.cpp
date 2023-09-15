#include"tgepch.h"
#include"Scripting.h"
#include"TurboGE/Input.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>
#include"TurboGE/Scene/Components.h"

namespace TurboGE
{
    template<typename T>
    static std::string convertToScript(T&& file)
    {
        std::string fileString = std::forward<T>(file);
        auto pos = fileString.find_last_of('\\');
        auto fileWithExtension = fileString.substr(pos + 1);
        auto pos2 = fileWithExtension.find('.');
        return fileWithExtension.substr(0, pos2);
    }

    static std::string fetchScript()
    {
        PyTraceBack_Here(PyEval_GetFrame());
        PyObject* exc;
        PyObject* val;
        PyObject* tb;
        PyErr_Fetch(&exc, &val, &tb);
        return convertToScript(PyUnicode_AsUTF8(PyObject_GetAttrString(PyObject_GetAttrString(PyObject_GetAttrString(tb, "tb_frame"), "f_code"), "co_filename")));
    }

    static PyObject* GetKeyPress(PyObject* pyObj, PyObject* args, PyObject* keywords)
    {
        int key;
        if (!PyArg_ParseTuple(args, "i", &key))
        {
            TURBO_CLIENT_WRN("GetKeyPress: Invalid argument");
            return NULL;
        }
        const bool isKeyPressed = Input::isKeyPressed((Key)key);
        return PyBool_FromLong(isKeyPressed);
    }


    static PyObject* GetTransform(PyObject* pyObj, PyObject* args, PyObject* keywords)
    {
        std::string filename = fetchScript();
        auto  it = PyScriptRepo::scriptMap.find(filename);
        if (it == PyScriptRepo::scriptMap.end())
        {
            TURBO_ASSERT("Could not find script", 0);
        }
        auto script = it->second;
        auto e = script->entity;
        auto transform = e.GetComponent<TransformComponent>();
        PyObject* result = Py_BuildValue("(ff)", transform.translate.x, transform.translate.y);
        return result;
    }

    static PyObject* SetTransform(PyObject* pyObj, PyObject* args, PyObject* keywords)
    {
        float x, y;
        if (!PyArg_ParseTuple(args, "ff", &x, &y))
        {
            TURBO_CLIENT_WRN("SetTransform: Invalid arguments");
            Py_RETURN_NONE;
        }

        std::string filename = fetchScript();
        auto  it = PyScriptRepo::scriptMap.find(filename);
        if (it == PyScriptRepo::scriptMap.end())
        {
            TURBO_ASSERT("Could not find script", 0);
        }
        auto script = it->second;
        auto e = script->entity;
        auto& transform = e.GetComponent<TransformComponent>();
        transform.translate.x = x;
        transform.translate.y = y;

        Py_RETURN_NONE;

    }

    static PyObject* ApplyForce(PyObject* pyObj, PyObject* args, PyObject* keywords)
    {
        float x,y;
        if (!PyArg_ParseTuple(args, "ff", &x, &y))
        {
            TURBO_CLIENT_WRN("ApplyForce: Invalid arguments");
            Py_RETURN_NONE;
        }

        std::string filename = fetchScript();
        auto  it = PyScriptRepo::scriptMap.find(filename);
        if (it == PyScriptRepo::scriptMap.end())
        {
            TURBO_ASSERT("Could not find script", 0);
        }
        auto script = it->second;
        auto e = script->entity;
        auto& rb = e.GetComponent<Rigidbody2D>();
        
        Physics2D::ApplyLinearImpulse(rb.body, x, y);

        Py_RETURN_NONE;
    }


    static PyMethodDef ComponentModifierMethods[] = {
       {"GetKeyPress", (PyCFunction)GetKeyPress, METH_KEYWORDS | METH_VARARGS, "Checks if the key provided in the argument is pressed. Return value: True if key is pressed."},
       {"GetTransform", (PyCFunction)GetTransform, METH_KEYWORDS | METH_VARARGS, "Gets the transform data of the entity. Return value: Transform structure. "},
       {"SetTransform", (PyCFunction)SetTransform, METH_KEYWORDS | METH_VARARGS, "Modifies transform with the argument provided. Return value: void"},
       {"ApplyForce", (PyCFunction)ApplyForce, METH_KEYWORDS | METH_VARARGS, "Apply a linear impulse on the entity. Return value: void"},
       {NULL, NULL, 0, NULL}  // list terminator
    };

    PyDoc_STRVAR(document, "This module is used to modify or get components of entity to which this script is attached");

    static struct PyModuleDef ComponentModifierModuleDefinition = {
       PyModuleDef_HEAD_INIT,
       "Component_Modifier",
       document,
       -1,
       ComponentModifierMethods,
       NULL,
       NULL,
       NULL,
       NULL
    };





    PyScript::PyScript(const std::string& scriptName)
    {
        CreateScript(scriptName);

        std::filesystem::path scriptPath = std::filesystem::current_path() / "scripts";
        std::filesystem::path modulePath = std::filesystem::current_path() / "scripts/modules";
        PyObject* sys = PyImport_ImportModule("sys");
        PyObject* sys_path = PyObject_GetAttrString(sys, "path");
        PyObject* folder_path = PyUnicode_FromString(scriptPath.string().c_str());
        PyObject* module_path = PyUnicode_FromString(modulePath.string().c_str());
        PyList_Append(sys_path, folder_path);
        PyList_Append(sys_path, module_path);

        Py_XDECREF(sys);
        Py_XDECREF(sys_path);
        Py_XDECREF(folder_path);
        Py_XDECREF(module_path);
    }
	void PyScript::Init()
	{
        //Py_SetPythonHome(L"C:\\Program Files\\Python311");
		Py_Initialize();

        PyObject* csMod = PyModule_Create(&ComponentModifierModuleDefinition);
        if (csMod)
        {
            PyObject* modules = PyImport_GetModuleDict();
            if (modules)
            {
                PyObject* nameObj = PyUnicode_FromString("Component_Modifier");
                if ((nameObj == NULL) || (PyObject_SetItem(modules, nameObj, csMod) != 0))
                    TURBO_CORE_ERR("PyObject_SetItem() failed!");
            }
            else
                TURBO_CORE_ERR("PyImport_GetModuleDict() returned NULL!");
        }
        else
            TURBO_CORE_ERR("Unabled to create Component_Modifier Python module!");

        if (PyImport_ImportModule("Component_Modifier") == NULL)
            TURBO_CORE_ERR("Unable to import Component_Modifier Python module!");


	}

    void PyScript::ShutDown()
    {
        
        PyScriptRepo::scriptMap.clear();
        Py_Finalize();
    }

    void PyScript::CreateScript(const std::string& scriptName)
    {
        m_ScriptName = scriptName;

        std::string pyScript = m_ScriptName + ".py";
        std::string fpath = "scripts/" + pyScript;
        std::filesystem::path path{ fpath };
        if (!std::filesystem::exists(fpath))
        {
            std::ofstream ofs(path);
            ofs << "import Component_Modifier\ndef OnCreate() :\n    print('Created')\n\ndef OnUpdate(ts) :\n    print('Time is ' + str(ts))\n\n";   
            ofs.close();
        }        
    }

    void PyScript::OnCreate()
    {
        std::string pyScript = m_ScriptName + ".py";
        std::string fpath = "scripts/" + pyScript;

        m_File = fopen(fpath.c_str(), "rb");
        if (m_File != NULL)
        {
            fseek(m_File, 0, SEEK_END);
            long fileSize = ftell(m_File);
            fseek(m_File, 0, SEEK_SET);
            char* scriptBuffer = new char[fileSize + 1];
            fread(scriptBuffer, sizeof(char), fileSize, m_File);
            scriptBuffer[fileSize] = '\0';


            PyObject* compiledCode = Py_CompileString(scriptBuffer, fpath.c_str(), Py_file_input);
            delete[] scriptBuffer;
            if (compiledCode == nullptr) {
                PyErr_Print();
                Py_DECREF(compiledCode);
                return;
            }
            else
            {
                Py_DECREF(compiledCode);
            }
            PyRun_SimpleFile(m_File, pyScript.c_str());
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to open Python script {0}\n", pyScript);
            return;
        }
        
        m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        m_Module = PyImport_Import(m_ModuleName);

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
                return;
            }
            Py_XDECREF(OnCreateFunc);
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to import Python module\n");
            return;
        }
    }

    void PyScript::OnUpdate(float ts)
    {
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
                return;
            }
            Py_XDECREF(OnUpdateFunc);
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to import Python module\n");
            return;
        }
    }
	
    PyScript::~PyScript()
    {
        if(m_File)
            fclose(m_File);
        Py_XDECREF(m_Module);
        Py_XDECREF(m_ModuleName);
    }
}