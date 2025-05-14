#include <Python.h>

static struct PyModuleDef mod = {
    PyModuleDef_HEAD_INIT,
    "_bootstrap",
    "No-op module; exists only to mark the wheel as binary.",
    -1,
    NULL
};

PyMODINIT_FUNC
PyInit__bootstrap(void)
{
    // returns an empty module object
    return PyModule_Create(&mod);
}
