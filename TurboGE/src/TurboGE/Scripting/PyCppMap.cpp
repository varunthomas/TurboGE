#include"tgepch.h"
#include"PyCppMap.h"
#include<Python.h>
#include"TurboGE/Input.h"

namespace TurboGE
{
    static PyObject* py_is_key_pressed(PyObject* self, PyObject* args)
    {
        int key;
        if (!PyArg_ParseTuple(args, "i", &key)) {
            return NULL;
        }
        bool result = Input::isKeyPressed((Key)key);
        return PyBool_FromLong(result);
    }

    // Method table
    static PyMethodDef myModuleMethods[] = {
        {"isKeyPressed", py_is_key_pressed, METH_VARARGS, "Check if key is pressed"},
        {NULL, NULL, 0, NULL} // Sentinel
    };

    // Module definition
    static struct PyModuleDef myModule = {
        PyModuleDef_HEAD_INIT,
        "mymodule", // Module name
        NULL,
        -1,
        myModuleMethods // Method table
    };

    // Module initialization function
    PyMODINIT_FUNC PyInit_mymodule(void) {
        return PyModule_Create(&myModule);
    }
}