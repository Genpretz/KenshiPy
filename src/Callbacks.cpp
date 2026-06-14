#include "Callbacks.h"
#include "Logger.h"
#include "SwigPyRuntime.h"

#include "kenshi\Character.h"
#include "kenshi\Inventory.h"

#include <Python.h>
#include <Ogre.h>

#include <vector>
#include <string>


// ---------------------------------------------------------------------------
// Callback registry macro toolkit
//
// DEFINE_CALLBACK_LIST(Name) generates the storage vector plus
// Register##Name##Callback / Unregister##Name##Callback — identical for
// every callback type regardless of signature.
//
// DEFINE_CALL_* macros generate Call##Name##Callbacks for a specific
// "shape" of signature. Combine list + call macros to get the full
// register/unregister/call trio for common shapes (see the combined
// macros below).
// ---------------------------------------------------------------------------

#define DEFINE_CALLBACK_LIST(Name) \
    static std::vector<PyObject*> g_##Name##Callbacks; \
    \
    void Register##Name##Callback(PyObject* callable) \
    { \
        if (!callable || !PyCallable_Check(callable)) \
        { \
            Logger::Error("Register" #Name "Callback: argument is not callable"); \
            return; \
        } \
        Py_INCREF(callable); \
        g_##Name##Callbacks.push_back(callable); \
    } \
    \
    void Unregister##Name##Callback(PyObject* callable) \
    { \
        for (std::vector<PyObject*>::iterator it = g_##Name##Callbacks.begin(); \
            it != g_##Name##Callbacks.end(); ++it) \
        { \
            if (*it == callable) \
            { \
                Py_DECREF(*it); \
                g_##Name##Callbacks.erase(it); \
                return; \
            } \
        } \
    }

// --- Call-function shapes -------------------------------------------------

// Single primitive value, e.g. CallKeyDownCallbacks(int), CallFrameCallbacks(float)
#define DEFINE_CALL_1VALUE(Name, CType, ToPyObj) \
    void Call##Name##Callbacks(CType val) \
    { \
        if (g_##Name##Callbacks.empty()) \
            return; \
        PyGILState_STATE gs = PyGILState_Ensure(); \
        PyObject* pyVal = ToPyObj(val); \
        if (!pyVal) \
        { \
            PyErr_Clear(); \
            PyGILState_Release(gs); \
            return; \
        } \
        for (std::vector<PyObject*>::iterator it = g_##Name##Callbacks.begin(); \
            it != g_##Name##Callbacks.end(); ++it) \
        { \
            PyObject* result = PyObject_CallFunctionObjArgs(*it, pyVal, nullptr); \
            if (!result) PyErr_Clear(); else Py_DECREF(result); \
        } \
        Py_DECREF(pyVal); \
        PyGILState_Release(gs); \
    }

// Single wrapped pointer, e.g. CallCharacterSelectCallbacks(Character*)
#define DEFINE_CALL_1PTR(Name, PtrType, WrapFn) \
    void Call##Name##Callbacks(PtrType* obj) \
    { \
        if (g_##Name##Callbacks.empty()) \
            return; \
        PyGILState_STATE gs = PyGILState_Ensure(); \
        PyObject* pyObj = WrapFn(obj); \
        if (!pyObj) \
        { \
            PyErr_Clear(); \
            PyGILState_Release(gs); \
            return; \
        } \
        for (std::vector<PyObject*>::iterator it = g_##Name##Callbacks.begin(); \
            it != g_##Name##Callbacks.end(); ++it) \
        { \
            PyObject* result = PyObject_CallFunctionObjArgs(*it, pyObj, nullptr); \
            if (!result) PyErr_Clear(); else Py_DECREF(result); \
        } \
        Py_DECREF(pyObj); \
        PyGILState_Release(gs); \
    }

// Wrapped pointer + primitive value, e.g. CallTakeMoneyCallbacks(Inventory*, int)
#define DEFINE_CALL_PTR_VALUE(Name, PtrType, WrapFn, ValType, ToPyObj) \
    void Call##Name##Callbacks(PtrType* obj, ValType val) \
    { \
        if (g_##Name##Callbacks.empty()) \
            return; \
        PyGILState_STATE gs = PyGILState_Ensure(); \
        PyObject* pyObj = WrapFn(obj); \
        PyObject* pyVal = ToPyObj(val); \
        if (!pyObj || !pyVal) \
        { \
            Py_XDECREF(pyObj); Py_XDECREF(pyVal); \
            PyErr_Clear(); \
            PyGILState_Release(gs); \
            return; \
        } \
        for (std::vector<PyObject*>::iterator it = g_##Name##Callbacks.begin(); \
            it != g_##Name##Callbacks.end(); ++it) \
        { \
            PyObject* result = PyObject_CallFunctionObjArgs(*it, pyObj, pyVal, nullptr); \
            if (!result) PyErr_Clear(); else Py_DECREF(result); \
        } \
        Py_DECREF(pyObj); Py_DECREF(pyVal); \
        PyGILState_Release(gs); \
    }

// Wrapped pointer + std::string, e.g. CallCharacterSayCallbacks(Character*, std::string)
#define DEFINE_CALL_PTR_STRING(Name, PtrType, WrapFn) \
    void Call##Name##Callbacks(PtrType* obj, const std::string& msg) \
    { \
        if (g_##Name##Callbacks.empty()) \
            return; \
        PyGILState_STATE gs = PyGILState_Ensure(); \
        PyObject* pyObj = WrapFn(obj); \
        PyObject* pyMsg = PyUnicode_FromString(msg.c_str()); \
        if (!pyObj || !pyMsg) \
        { \
            Py_XDECREF(pyObj); Py_XDECREF(pyMsg); \
            PyErr_Clear(); \
            PyGILState_Release(gs); \
            return; \
        } \
        for (std::vector<PyObject*>::iterator it = g_##Name##Callbacks.begin(); \
            it != g_##Name##Callbacks.end(); ++it) \
        { \
            PyObject* result = PyObject_CallFunctionObjArgs(*it, pyObj, pyMsg, nullptr); \
            if (!result) PyErr_Clear(); else Py_DECREF(result); \
        } \
        Py_DECREF(pyObj); Py_DECREF(pyMsg); \
        PyGILState_Release(gs); \
    }

// --- Combined macros: list + register + unregister + call, in one line ----

#define DEFINE_CALLBACK_1VALUE(Name, CType, ToPyObj) \
    DEFINE_CALLBACK_LIST(Name) \
    DEFINE_CALL_1VALUE(Name, CType, ToPyObj)

#define DEFINE_CALLBACK_1PTR(Name, PtrType, WrapFn) \
    DEFINE_CALLBACK_LIST(Name) \
    DEFINE_CALL_1PTR(Name, PtrType, WrapFn)

#define DEFINE_CALLBACK_PTR_VALUE(Name, PtrType, WrapFn, ValType, ToPyObj) \
    DEFINE_CALLBACK_LIST(Name) \
    DEFINE_CALL_PTR_VALUE(Name, PtrType, WrapFn, ValType, ToPyObj)

#define DEFINE_CALLBACK_PTR_STRING(Name, PtrType, WrapFn) \
    DEFINE_CALLBACK_LIST(Name) \
    DEFINE_CALL_PTR_STRING(Name, PtrType, WrapFn)


// ---------------------------------------------------------------------------
// SWIG pointer wrapper generator
//
// Generates WrapXPointer(Type* p) that converts a raw pointer to the
// corresponding Python object via SWIG, with a raw-address fallback if the
// SWIG type can't be resolved (so callbacks can still run).
// ---------------------------------------------------------------------------

#define DEFINE_SWIG_POINTER_WRAPPER(Name, CppType, SwigTypeName) \
    static swig_type_info* Resolve##Name##SwigType() \
    { \
        static swig_type_info* g_type = nullptr; \
        static bool g_resolved = false; \
        if (!g_resolved) \
        { \
            g_type = SWIG_Python_TypeQuery(SwigTypeName); \
            g_resolved = true; \
        } \
        return g_type; \
    } \
    \
    static PyObject* Wrap##Name##Pointer(CppType* p) \
    { \
        if (!p) \
        { \
            Py_INCREF(Py_None); \
            return Py_None; \
        } \
        swig_type_info* type = Resolve##Name##SwigType(); \
        if (!type) \
            return PyLong_FromVoidPtr(static_cast<void*>(p)); \
        return SWIG_Python_NewPointerObj(nullptr, static_cast<void*>(p), type, 0); \
    }

DEFINE_SWIG_POINTER_WRAPPER(Character, Character, "Character *")
DEFINE_SWIG_POINTER_WRAPPER(Inventory, Inventory, "Inventory *")

// Single value callbacks
DEFINE_CALLBACK_1VALUE(KeyDown, int, PyLong_FromLong)

// Single-pointer Character callbacks
DEFINE_CALLBACK_1PTR(CharacterSelect, Character, WrapCharacterPointer)
DEFINE_CALLBACK_1PTR(CharacterUnselect, Character, WrapCharacterPointer)
DEFINE_CALLBACK_1PTR(CharacterDeclareDead, Character, WrapCharacterPointer)

// Pointer + string
DEFINE_CALLBACK_PTR_STRING(CharacterSay, Character, WrapCharacterPointer)

// Pointer + value
DEFINE_CALLBACK_PTR_VALUE(TakeMoney, Inventory, WrapInventoryPointer, int, PyLong_FromLong)

// ---------------------------------------------------------------------------
// Frame callbacks — special case
//
// Unlike the other callback types, Frame needs to hook into Ogre's per-frame
// event system rather than a MinHook detour. We only keep an
// Ogre::FrameListener registered while at least one Python callback exists,
// so idle scripts cost nothing.
// ---------------------------------------------------------------------------

static std::vector<PyObject*> g_FrameCallbacks;

// Generates CallFrameCallbacks(float) — iterates g_FrameCallbacks above.
DEFINE_CALL_1VALUE(Frame, float, PyFloat_FromDouble)

class FrameCallbackListener : public Ogre::FrameListener
{
public:
    bool frameStarted(const Ogre::FrameEvent& evt)
    {
        CallFrameCallbacks(evt.timeSinceLastFrame);
        return true;
    }
};

static FrameCallbackListener g_frameListener;
static bool g_frameListenerInstalled = false;

void RegisterFrameCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterFrameCallback: argument is not callable");
        return;
    }

    Py_INCREF(callable);
    g_FrameCallbacks.push_back(callable);

    if (!g_frameListenerInstalled)
    {
        Ogre::Root::getSingleton().addFrameListener(&g_frameListener);
        g_frameListenerInstalled = true;
    }
}

void UnregisterFrameCallback(PyObject* callable)
{
    for (std::vector<PyObject*>::iterator it = g_FrameCallbacks.begin();
        it != g_FrameCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_FrameCallbacks.erase(it);
            break;
        }
    }

    if (g_FrameCallbacks.empty() && g_frameListenerInstalled)
    {
        Ogre::Root::getSingleton().removeFrameListener(&g_frameListener);
        g_frameListenerInstalled = false;
    }
}