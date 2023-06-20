# PyCppMap.pyx
cdef extern from "PyCppMap.h":
    int add_numbers(int a, int b)

def py_add_numbers(int a, int b):
    return add_numbers(a, b)