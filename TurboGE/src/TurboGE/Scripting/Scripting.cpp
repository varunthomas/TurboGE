#include"tgepch.h"
#include"Scripting.h"
#include"TurboGE/Input.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <filesystem>
//#include <stdio.h>

namespace TurboGE
{
    static PyObject* GetKeyPress(PyObject* pyObj, PyObject* args, PyObject* keywords)
    {
        const bool isKeyPressed = Input::isKeyPressed(Key::A);
        printf("C Function GetRandomNumber():  I choose:  %i\n", isKeyPressed);
        return PyBool_FromLong(isKeyPressed);
    }

    static PyMethodDef DemoEditorMethods[] = {
       {"GetKeyPress", (PyCFunction)GetKeyPress, METH_KEYWORDS | METH_VARARGS, "Returns a random integer."},
       {NULL, NULL, 0, NULL}  // list terminator
    };

    PyDoc_STRVAR(demo_doc, "This module is just here to demonstrate calling a C function from Python and having it return a value.");

    static struct PyModuleDef DemoModuleDefinition = {
       PyModuleDef_HEAD_INIT,
       "embedded_demo",
       demo_doc,
       -1,
       DemoEditorMethods,
       NULL,
       NULL,
       NULL,
       NULL
    };





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









        //m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        //m_Module = PyImport_Import(m_ModuleName);


        

    }
	void PyScript::Init()
	{
        //std::filesystem::current_path("scripts");
		Py_Initialize();


        PyObject* csMod = PyModule_Create(&DemoModuleDefinition);
        if (csMod)
        {
            PyObject* modules = PyImport_GetModuleDict();
            if (modules)
            {
                PyObject* nameObj = PyUnicode_FromString("embedded_demo");
                if ((nameObj == NULL) || (PyObject_SetItem(modules, nameObj, csMod) != 0)) printf("PyObject_SetItem() failed!?\n");
            }
            else printf("PyImport_GetModuleDict() returned NULL!\n");
        }
        else printf("Unabled to create embedded_demo Python module!\n");

        if (PyImport_ImportModule("embedded_demo") == NULL) printf("Unable to import embedded_demo Python module!\n");


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
        
        PyScriptRepo::scriptMap.clear();
        std::cout << "Shutdown called\n";
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
        std::string pyScript = m_ScriptName + ".py";
        std::string fpath = "scripts/" + pyScript;
        std::filesystem::path path{ fpath };
        std::ofstream ofs(path);
        //ofs << "import embedded_demo\ndef OnCreate():\n\tprint('Created')\n\ndef OnUpdate(ts):\n\tprint('Time is ' + str(ts))\n\tv = embedded_demo.GetKeyPress()\n\tprint('Python Script:  The random value I got from the C GetRandomNumber() function is: %i' % v)\n";
        ofs << "def OnCreate():\n    print('Created')\n\ndef OnUpdate(ts):\n    print('Time is ' + str(ts))\n\n";
        //ofs << "def OnCreate():\n\tprint('Created')\n\ndef OnUpdate(ts):\n\tprint('Time is ' + str(ts))";

        ofs.close();

        
        //m_File = fopen(fpath.c_str(), "rb");
        //if (m_File != NULL)
        {



           

          //  std::cout << "Complete import\n";
            //PyRun_SimpleFile(m_File, pyScript.c_str());
        }
        //else
        {
          //  TURBO_CLIENT_ERR("Failed to open Python script {0}\n", pyScript);
           // return;
        }

        
    }

    void PyScript::OnCreate()
    {

        std::string pyScript = m_ScriptName + ".py";
        std::string fpath = "scripts/" + pyScript;



        
        FILE* file = fopen(fpath.c_str(), "rb");
        if (file == nullptr) {
            std::cerr << "Failed to open script file: " << fpath << std::endl;
            Py_Finalize();
            return;
        }
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* scriptBuffer = new char[fileSize + 1];
        fread(scriptBuffer, sizeof(char), fileSize, file);
        scriptBuffer[fileSize] = '\0';
        fclose(file);

        // Compile the script to check for syntax errors
        PyObject* compiledCode = Py_CompileString(scriptBuffer, fpath.c_str(), Py_file_input);
        delete[] scriptBuffer;
        if (compiledCode == nullptr) {
            PyErr_Print();
            std::cerr << "Script has syntax errors." << std::endl;
            return;
        }
        else {
            std::cout << "Script is syntaxically correct." << std::endl;
            Py_DECREF(compiledCode);
        }





        /*std::stringstream output;
        FILE* pipe = _popen(("python " + fpath + " 2>&1").c_str(), "r");
        if (pipe) {
            char buffer[128];
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != nullptr) {
                    output << buffer;
                }
            }
            _pclose(pipe);
        }
        if(!output.str().empty())
            std::cout << "************ " << output.str() << std::endl;

            */





        PyObject* obj = Py_BuildValue("s", fpath.c_str());
        if (obj == NULL)
        {
            if (PyErr_Occurred())
                PyErr_Print();
        }
        m_File = _Py_fopen_obj(obj, "r+");
        //m_File = fopen(fpath.c_str(), "rb");
        if (m_File != NULL)
        {





            std::cout << "Complete import\n";
             
            try
            {
                PyRun_SimpleFile(m_File, pyScript.c_str());
                //std::cout << "Failed\n";
                if (PyErr_Occurred())
                    PyErr_Print();
            }
            catch (...)
            {
                if (PyErr_Occurred())
                    PyErr_Print();
            }
        }
        else
        {
            TURBO_CLIENT_ERR("Failed to open Python script {0}\n", pyScript);
            return;
        }
        
        m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        m_Module = PyImport_Import(m_ModuleName);

        //SetPath();
        

        // Call the hello() function from Python
        //PyObject* moduleName = PyUnicode_FromString(scriptName.c_str());
        //m_ModuleName = PyUnicode_FromString(m_ScriptName.c_str());
        //m_Module = PyImport_Import(m_ModuleName);
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

        //UnsetPath();

    }

    void PyScript::OnUpdate(float ts)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        //SetPath();

        //std::string pyScript = m_ScriptName + ".py";

        //FILE* file = fopen(pyScript.c_str(), "rb");
        //if (file)
        //{
            //PyRun_SimpleFile(m_File, pyScript.c_str());
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

        //UnsetPath();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << "\n";
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << "\n\n";

    }
	
    PyScript::~PyScript()
    {
        std::cout << "Dest pyscript\n";
        if(m_File)
            fclose(m_File);
        Py_XDECREF(m_Module);
        Py_XDECREF(m_ModuleName);
    }
}