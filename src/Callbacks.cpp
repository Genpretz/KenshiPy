//Python 3.4
#include <Python.h>

//KenshiLib
#include <Debug.h>
#include <Ogre.h>

#include <vector>

// ----------------------------------------------------------------------------
// Key down callbacks
// ----------------------------------------------------------------------------

static std::vector<PyObject*> g_keyDownCallbacks;

void RegisterKeyDownCallback(PyObject* callable)
{
	if (!callable || !PyCallable_Check(callable))
	{
		ErrorLog("KenshiPy: registerKeyDownCallback requires a callable");
		return;
	}
	Py_INCREF(callable);
	g_keyDownCallbacks.push_back(callable);
}

void UnregisterKeyDownCallback(PyObject* callable)
{
	for (std::vector<PyObject*>::iterator it = g_keyDownCallbacks.begin();
		it != g_keyDownCallbacks.end(); ++it)
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

	PyGILState_STATE gstate = PyGILState_Ensure();

	PyObject* pyKey = PyLong_FromLong(keyCode);
	for (size_t i = 0; i < g_keyDownCallbacks.size(); ++i)
	{
		PyObject* result = PyObject_CallFunctionObjArgs(g_keyDownCallbacks[i], pyKey, NULL);
		if (!result)
			PyErr_Clear();
		else
			Py_DECREF(result);
	}
	Py_DECREF(pyKey);

	PyGILState_Release(gstate);
}

// ----------------------------------------------------------------------------
// Frame/tick callbacks
// ----------------------------------------------------------------------------
void CallFrameCallbacks(float deltaTime); //forward declaration

static std::vector<PyObject*> g_frameCallbacks;
static Ogre::FrameListener* g_pyFrameListener = nullptr;
static Ogre::Root* g_root = nullptr;

class PythonFrameListener : public Ogre::FrameListener
{
public:
	bool frameStarted(const Ogre::FrameEvent& evt) override
	{
		CallFrameCallbacks(evt.timeSinceLastFrame);
		return true;
	}
};

void RegisterFrameCallback(PyObject* callable)
{
	if (!callable || !PyCallable_Check(callable))
	{
		ErrorLog("KenshiPy: registerFrameCallback requires a callable");
		return;
	}

	Py_INCREF(callable);
	g_frameCallbacks.push_back(callable);

	// Lazy init listener
	if (!g_pyFrameListener)
	{
		g_pyFrameListener = new PythonFrameListener();
		g_root->addFrameListener(g_pyFrameListener);
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

	// Auto-remove listener if unused
	if (g_frameCallbacks.empty() && g_pyFrameListener)
	{
		g_root->removeFrameListener(g_pyFrameListener);
		delete g_pyFrameListener;
		g_pyFrameListener = nullptr;
	}
}

void CallFrameCallbacks(float deltaTime)
{
	if (g_frameCallbacks.empty())
		return;

	PyGILState_STATE gstate = PyGILState_Ensure();

	PyObject* pyDeltaTime = PyFloat_FromDouble(deltaTime);
	for (size_t i = 0; i < g_frameCallbacks.size(); ++i)
	{
		PyObject* result = PyObject_CallFunctionObjArgs(g_frameCallbacks[i], pyDeltaTime, NULL);
		if (!result)
			PyErr_Clear();
		else
			Py_DECREF(result);
	}
	Py_DECREF(pyDeltaTime);

	PyGILState_Release(gstate);
}