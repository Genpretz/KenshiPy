#include "Callbacks.h"
#include "Logger.h"

#include <Python.h>
#include <Ogre.h>

#include <vector>

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
