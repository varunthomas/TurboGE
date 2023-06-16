#include <iostream>
#include <Python.h>
#include"TurboGE/Input.h"

extern "C"
{



// The C++ function you want to call from Python
int my_cpp_function(int arg1, int arg2) {
    // Your C++ code here
    // ...
    int x = TurboGE::Input::isKeyPressed(TurboGE::Key::A);
    int result = arg1 + arg2;
    return result;
}

// Python wrapper function for the C++ function
static PyObject* py_my_cpp_function(PyObject* self, PyObject* args) {

    PyGILState_STATE gstate = PyGILState_Ensure();
    int arg1, arg2;
    if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2)) {
        return NULL;
    }

    int result = my_cpp_function(arg1, arg2);
    PyObject* r = PyLong_FromLong(result);
    PyGILState_Release(gstate);
    return r;
}

// Module method table
static PyMethodDef ScriptEngineMethods[] = {
    {"my_cpp_function", py_my_cpp_function, METH_VARARGS, "Call the C++ function"},
    {NULL, NULL, 0, NULL}  // Sentinel
};

// Module definition
static struct PyModuleDef ScriptEnginemodule = {
    PyModuleDef_HEAD_INIT,
    "ScriptEngine",   // Module name
    NULL,         // Module documentation
    -1,           // Size of per-interpreter state of the module,
    ScriptEngineMethods
};

// Module initialization function
PyMODINIT_FUNC PyInit_ScriptEngine(void) {
    return PyModule_Create(&ScriptEnginemodule);
}

}
