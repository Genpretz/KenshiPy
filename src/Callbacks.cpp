#include "Callbacks.h"
#include "Logger.h"
#include "SwigPyRuntime.h"

#include "kenshi\Character.h"

#include <Python.h>
#include <Ogre.h>

#include <vector>
#include <string>

// ---------------------------------------------------------------------------
// SWIG runtime helpers
// ---------------------------------------------------------------------------
// The module's SWIG wrapper code provides these symbols. We use them to wrap
// raw Kenshi pointers (e.g. Character*) into the corresponding Python objects
// (e.g. KenshiPy.Character).
struct swig_type_info;
swig_type_info* SWIG_Python_TypeQuery(const char* type);
PyObject* SWIG_Python_NewPointerObj(PyObject* self, void* ptr, swig_type_info* type, int flags);

static swig_type_info* ResolveCharacterSwigType()
{
    static swig_type_info* g_type = nullptr;
    static bool g_resolved = false;
    if (!g_resolved)
    {
        // This must match the SWIG type name for Character* in the generated wrapper.
        g_type = SWIG_Python_TypeQuery("Character *");
        g_resolved = true;
    }
    return g_type;
}

static PyObject* WrapCharacterPointer(Character* c)
{
    if (!c)
    {
        Py_INCREF(Py_None);
        return Py_None;
    }

    swig_type_info* type = ResolveCharacterSwigType();
    if (!type)
    {
        // Fallback: return a raw address as an int (so callbacks can still run).
        return PyLong_FromVoidPtr(static_cast<void*>(c));
    }

    // flags=0: non-owning reference to an existing in-game object.
    return SWIG_Python_NewPointerObj(nullptr, static_cast<void*>(c), type, 0);
}




// ---------------------------------------------------------------------------
// Key-down callbacks
// ---------------------------------------------------------------------------

static std::vector<PyObject*> g_keyDownCallbacks;

void RegisterKeyDownCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterKeyDownCallback: argument is not callable");
        return;
    }
    Py_INCREF(callable);
    g_keyDownCallbacks.push_back(callable);
}

void UnregisterKeyDownCallback(PyObject* callable)
{
    for (auto it = g_keyDownCallbacks.begin(); it != g_keyDownCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_keyDownCallbacks.erase(it);
            return;
        }
    }
}

void CallKeyDownCallbacks(int keyCode)
{
    if (g_keyDownCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();

    PyObject* pyKey = PyLong_FromLong(keyCode);

    for (std::vector<PyObject*>::iterator it = g_keyDownCallbacks.begin();
        it != g_keyDownCallbacks.end();
        ++it)
    {
        PyObject* cb = *it;

        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyKey, nullptr);

        if (!result)
            PyErr_Clear();   // don't let one bad callback kill the rest
        else
            Py_DECREF(result);
    }

    Py_DECREF(pyKey);
    PyGILState_Release(gs);
}

// ---------------------------------------------------------------------------
// Character::say callbacks — callable receives (Character, str)
// ---------------------------------------------------------------------------
static std::vector<PyObject*> g_characterSayCallbacks;

void RegisterCharacterSayCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterCharacterSayCallback: argument is not callable");
        return;
    }
    Py_INCREF(callable);
    g_characterSayCallbacks.push_back(callable);
}

void UnregisterCharacterSayCallback(PyObject* callable)
{
    for (auto it = g_characterSayCallbacks.begin(); it != g_characterSayCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_characterSayCallbacks.erase(it);
            return;
        }
    }
}

void CallCharacterSayCallbacks(Character* c, const std::string& msg)
{
    if (g_characterSayCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();

    PyObject* pyChar = WrapCharacterPointer(c);
    PyObject* pyMsg = PyUnicode_FromString(msg.c_str());

    if (!pyChar || !pyMsg)
    {
        Py_XDECREF(pyChar);
        Py_XDECREF(pyMsg);
        PyErr_Clear();
        PyGILState_Release(gs);
        return;
    }

    for (auto it = g_characterSayCallbacks.begin(); it != g_characterSayCallbacks.end(); ++it)
    {
        PyObject* cb = *it;
        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyChar, pyMsg, nullptr);
        if (!result)
            PyErr_Clear();
        else
            Py_DECREF(result);
    }

    Py_DECREF(pyChar);
    Py_DECREF(pyMsg);
    PyGILState_Release(gs);
}

// ---------------------------------------------------------------------------
// Character::_NV_select callbacks — callable receives (Character)
// ---------------------------------------------------------------------------
static std::vector<PyObject*> g_characterSelectCallbacks;

void RegisterCharacterSelectCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterCharacterSelectCallback: argument is not callable");
        return;
    }
    Py_INCREF(callable);
    g_characterSelectCallbacks.push_back(callable);
}

void UnregisterCharacterSelectCallback(PyObject* callable)
{
    for (auto it = g_characterSelectCallbacks.begin(); it != g_characterSelectCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_characterSelectCallbacks.erase(it);
            return;
        }
    }
}

void CallCharacterSelectCallbacks(Character* c)
{
    if (g_characterSelectCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();
    PyObject* pyChar = WrapCharacterPointer(c);
    if (!pyChar)
    {
        PyErr_Clear();
        PyGILState_Release(gs);
        return;
    }

    for (auto it = g_characterSelectCallbacks.begin(); it != g_characterSelectCallbacks.end(); ++it)
    {
        PyObject* cb = *it;
        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyChar, nullptr);
        if (!result)
            PyErr_Clear();
        else
            Py_DECREF(result);
    }

    Py_DECREF(pyChar);
    PyGILState_Release(gs);
}

// ---------------------------------------------------------------------------
// Character::_NV_unselect callbacks — callable receives (Character)
// ---------------------------------------------------------------------------
static std::vector<PyObject*> g_characterUnselectCallbacks;

void RegisterCharacterUnselectCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterCharacterUnselectCallback: argument is not callable");
        return;
    }
    Py_INCREF(callable);
    g_characterUnselectCallbacks.push_back(callable);
}

void UnregisterCharacterUnselectCallback(PyObject* callable)
{
    for (auto it = g_characterUnselectCallbacks.begin(); it != g_characterUnselectCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_characterUnselectCallbacks.erase(it);
            return;
        }
    }
}

void CallCharacterUnselectCallbacks(Character* c)
{
    if (g_characterUnselectCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();
    PyObject* pyChar = WrapCharacterPointer(c);
    if (!pyChar)
    {
        PyErr_Clear();
        PyGILState_Release(gs);
        return;
    }

    for (auto it = g_characterUnselectCallbacks.begin(); it != g_characterUnselectCallbacks.end(); ++it)
    {
        PyObject* cb = *it;
        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyChar, nullptr);
        if (!result)
            PyErr_Clear();
        else
            Py_DECREF(result);
    }

    Py_DECREF(pyChar);
    PyGILState_Release(gs);
}

// ---------------------------------------------------------------------------
// Character::declareDead callbacks — callable receives (Character)
// ---------------------------------------------------------------------------
static std::vector<PyObject*> g_characterDeclareDeadCallbacks;

void RegisterCharacterDeclareDeadCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterCharacterDeclareDeadCallback: argument is not callable");
        return;
    }
    Py_INCREF(callable);
    g_characterDeclareDeadCallbacks.push_back(callable);
}

void UnregisterCharacterDeclareDeadCallback(PyObject* callable)
{
    for (auto it = g_characterDeclareDeadCallbacks.begin(); it != g_characterDeclareDeadCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_characterDeclareDeadCallbacks.erase(it);
            return;
        }
    }
}

void CallCharacterDeclareDeadCallbacks(Character* c)
{
    if (g_characterDeclareDeadCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();
    PyObject* pyChar = WrapCharacterPointer(c);
    if (!pyChar)
    {
        PyErr_Clear();
        PyGILState_Release(gs);
        return;
    }

    for (auto it = g_characterDeclareDeadCallbacks.begin(); it != g_characterDeclareDeadCallbacks.end(); ++it)
    {
        PyObject* cb = *it;
        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyChar, nullptr);
        if (!result)
            PyErr_Clear();
        else
            Py_DECREF(result);
    }

    Py_DECREF(pyChar);
    PyGILState_Release(gs);
}

// ---------------------------------------------------------------------------
// Frame callbacks
//
// We use an Ogre::FrameListener so we don't need to patch the game loop.
// Ogre::Root is a singleton — we retrieve it lazily rather than storing a
// pointer that could dangle or be unset at startup.
// ---------------------------------------------------------------------------

static std::vector<PyObject*> g_frameCallbacks;

static void CallFrameCallbacks(float deltaTime)
{
    if (g_frameCallbacks.empty())
        return;

    PyGILState_STATE gs = PyGILState_Ensure();

    PyObject* pyDt = PyFloat_FromDouble(static_cast<double>(deltaTime));
    for (std::vector<PyObject*>::iterator it = g_frameCallbacks.begin();
        it != g_frameCallbacks.end();
        ++it)
    {
        PyObject* cb = *it;

        PyObject* result = PyObject_CallFunctionObjArgs(cb, pyDt, nullptr);
        if (!result)
            PyErr_Clear();
        else
            Py_DECREF(result);
    }
    Py_DECREF(pyDt);

    PyGILState_Release(gs);
}

// Internal FrameListener — created on demand, destroyed when unused.
class PythonFrameListener : public Ogre::FrameListener
{
public:
    bool frameStarted(const Ogre::FrameEvent& evt) override
    {
        CallFrameCallbacks(evt.timeSinceLastFrame);
        return true;   // return false would stop the render loop

    }
};

static PythonFrameListener* g_frameListener = nullptr;

static Ogre::Root* GetOgreRoot()
{
    // Ogre::Root is a singleton — getSingletonPtr() returns nullptr if it
    // hasn't been created yet, so this is safe to call at any time.
    return Ogre::Root::getSingletonPtr();
}

void RegisterFrameCallback(PyObject* callable)
{
    if (!callable || !PyCallable_Check(callable))
    {
        Logger::Error("RegisterFrameCallback: argument is not callable");
        return;
    }

    Ogre::Root* root = GetOgreRoot();
    if (!root)
    {
        Logger::Error("RegisterFrameCallback: Ogre::Root not available yet");
        return;
    }

    Py_INCREF(callable);
    g_frameCallbacks.push_back(callable);

    if (!g_frameListener)
    {
        g_frameListener = new PythonFrameListener();
        root->addFrameListener(g_frameListener);
        Logger::Debug("PythonFrameListener registered.");
    }
}

void UnregisterFrameCallback(PyObject* callable)
{
    for (auto it = g_frameCallbacks.begin(); it != g_frameCallbacks.end(); ++it)
    {
        if (*it == callable)
        {
            Py_DECREF(*it);
            g_frameCallbacks.erase(it);
            break;
        }
    }

    // Remove the listener when no callbacks remain so we don't burn CPU.
    if (g_frameCallbacks.empty() && g_frameListener)
    {
        Ogre::Root* root = GetOgreRoot();
        if (root)
            root->removeFrameListener(g_frameListener);

        delete g_frameListener;
        g_frameListener = nullptr;
        Logger::Debug("PythonFrameListener removed (no remaining callbacks).");
    }
}

