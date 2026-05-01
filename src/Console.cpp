#include "Console.h"
#include "PyRuntime.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>

#include "mygui/MyGUI_EditBox.h"
#include "mygui/MyGUI_Gui.h"
#include "mygui/MyGUI_InputManager.h"
#include "mygui/MyGUI_ScrollView.h"
#include "mygui/MyGUI_TextBox.h"
#include "mygui/MyGUI_Window.h"

namespace Console
{
    bool g_consoleInitialized = false;
}

bool Console::IsInitialized()
{
    return g_consoleInitialized;
}

static MyGUI::Widget* FindWidget(MyGUI::EnumeratorWidgetPtr enumerator, const std::string& name)
{
    while (enumerator.next())
    {
        MyGUI::Widget* w = enumerator.current();
        const std::string& widgetName = w->getName();
        if (widgetName == name)
            return w;

        size_t splitPos = widgetName.find('_');
        if (splitPos != std::string::npos && widgetName.substr(splitPos + 1) == name)
            return w;

        if (w->getChildCount() > 0)
        {
            MyGUI::Widget* found = FindWidget(w->getEnumerator(), name);
            if (found != nullptr)
                return found;
        }
    }
    return nullptr;
}

void Console::InitFrameHandler(float /*timeDelta*/)
{
    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
    if (!gui)
        return;

    MyGUI::Widget* versionText = FindWidget(gui->getEnumerator(), "VersionText");
    if (!versionText)
        return;

    // GUI is ready, unregister frame handler and initialize console
    gui->eventFrameStart -= MyGUI::newDelegate(Console::InitFrameHandler);
    g_consoleInitialized = true;
    Console::Init();
}

static MyGUI::Window* g_consoleWindow = nullptr;
static MyGUI::ScrollView* g_outputScrollView = nullptr;
static MyGUI::TextBox* g_outputBox = nullptr;
static MyGUI::EditBox* g_inputBox = nullptr;

static PyObject* Redirector_write(PyObject* /*self*/, PyObject* args)
{
    const char* text = nullptr;
    if (!PyArg_ParseTuple(args, "s", &text))
        Py_RETURN_NONE;

    if (text && text[0] != '\0')
        Console::AppendOutput(text);

    Py_RETURN_NONE;
}

static PyObject* Redirector_flush(PyObject* /*self*/, PyObject* /*args*/)
{
    Py_RETURN_NONE;
}

static PyMethodDef g_redirectorMethods[] =
{
    { "write", Redirector_write, METH_VARARGS, "write(text)" },
    { "flush", Redirector_flush, METH_VARARGS, "flush()" },
    { nullptr, nullptr, 0, nullptr }
};

// run() builtin for Python console
static PyObject* Builtin_run(PyObject* /*self*/, PyObject* args)
{
    const char* path = nullptr;
    if (!PyArg_ParseTuple(args, "s", &path))
        Py_RETURN_NONE;

    std::string scriptPath(path);
    for (size_t i = 0; i < scriptPath.size(); ++i)
    {
        if (scriptPath[i] == '\\' || scriptPath[i] == '\t' || scriptPath[i] == '\n')
            scriptPath[i] = '/';
    }

    RunScript(scriptPath);
    Py_RETURN_NONE;
}

static PyMethodDef g_builtinRunDef = { "run", Builtin_run, METH_VARARGS, "run(path) - execute a Python script file" };

static void OnInputAccept(MyGUI::EditBox* sender)
{
    MyGUI::UString uinput = sender->getCaption();
    sender->setCaption("");

    if (uinput.empty())
        return;

    Console::AppendOutput(">>> " + uinput + "\n");

    std::string input = uinput.asUTF8();

    PyGILState_STATE gstate = PyGILState_Ensure();

    PyObject* code = Py_CompileString(input.c_str(), "<console>", Py_single_input);
    if (!code)
    {
        PyObject* ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

        if (pvalue)
        {
            PyObject* pstr = PyObject_Str(pvalue);
            if (pstr)
            {
                Console::AppendOutput(std::string(PyUnicode_AsUTF8(pstr)) + "\n");
                Py_DECREF(pstr);
            }
        }

        Py_XDECREF(ptype);
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
        PyGILState_Release(gstate);
        return;
    }

    PyObject* mainModule = PyImport_AddModule("__main__");
    PyObject* globals = PyModule_GetDict(mainModule);
    PyObject* result = PyEval_EvalCode(code, globals, globals);

    if (!result)
    {
        PyObject* ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);

        if (pvalue)
        {
            PyObject* pstr = PyObject_Str(pvalue);
            if (pstr)
            {
                Console::AppendOutput(std::string(PyUnicode_AsUTF8(pstr)) + "\n");
                Py_DECREF(pstr);
            }
        }

        Py_XDECREF(ptype);
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
    }

    Py_XDECREF(result);
    Py_DECREF(code);

    PyGILState_Release(gstate);
}

static void OnConsoleWindowButton(MyGUI::Window* sender, const std::string& name)
{
    if (name == "close")
        sender->setVisible(false);
}

void Console::Init()
{
    // Redirect Python stdout/stderr
    PyGILState_STATE gstate = PyGILState_Ensure();

    PyObject* mainModule = PyImport_AddModule("__main__");
    PyObject* globals = PyModule_GetDict(mainModule);

    PyObject* redirectorModule = PyModule_New("_console_redirector");
    PyMethodDef* writeDef = &g_redirectorMethods[0];
    PyMethodDef* flushDef = &g_redirectorMethods[1];

    PyObject* writeFunc = PyCFunction_New(writeDef, nullptr);
    PyObject* flushFunc = PyCFunction_New(flushDef, nullptr);

    if (writeFunc)
        PyModule_AddObject(redirectorModule, "write", writeFunc);
    if (flushFunc)
        PyModule_AddObject(redirectorModule, "flush", flushFunc);

    PyObject* sysModule = PyImport_ImportModule("sys");
    if (sysModule)
    {
        PyObject_SetAttrString(sysModule, "stdout", redirectorModule);
        PyObject_SetAttrString(sysModule, "stderr", redirectorModule);
        Py_DECREF(sysModule);
    }

    Py_DECREF(redirectorModule);

    PyObject* runFunc = PyCFunction_New(&g_builtinRunDef, nullptr);
    if (runFunc)
    {
        PyDict_SetItemString(globals, "run", runFunc);
        Py_DECREF(runFunc);
    }

    PyGILState_Release(gstate);

    // Build the MyGUI console window
    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
    if (!gui)
    {
        Logger::ErrorLog("Console::Init - MyGUI not ready");
        return;
    }

    const int W = 800, H = 400, inputH = 30, pad = 4;

    g_consoleWindow = gui->createWidget<MyGUI::Window>(
        "Kenshi_WindowCX", 100, 100, W, H,
        MyGUI::Align::Default, "Window", "KenshiPyConsole");

    g_consoleWindow->setCaption("KenshiPy Console");
    g_consoleWindow->setVisible(false);
    g_consoleWindow->eventWindowButtonPressed += MyGUI::newDelegate(OnConsoleWindowButton);

    MyGUI::Widget* client = g_consoleWindow->getClientWidget();
    int clientW = client->getWidth();
    int clientH = client->getHeight();
    int outputH = clientH - inputH - pad * 3;

    // Output area
    g_outputScrollView = client->createWidget<MyGUI::ScrollView>(
        "Kenshi_ScrollViewEmpty",
        MyGUI::IntCoord(pad, pad, clientW - pad * 2, outputH),
        MyGUI::Align::Stretch, "KenshiPyOutputScroll");

    g_outputScrollView->setCanvasSize(
        g_outputScrollView->getWidth() - 20,
        g_outputScrollView->getHeight() - 20);

    g_outputBox = g_outputScrollView->createWidget<MyGUI::TextBox>(
        "Kenshi_GenericTextBox",
        0, 0,
        g_outputScrollView->getCanvasSize().width,
        g_outputScrollView->getCanvasSize().height,
        MyGUI::Align::Stretch, "KenshiPyOutput");

    g_outputBox->setEnabled(false);

    // Input area
    g_inputBox = client->createWidget<MyGUI::EditBox>(
        "Kenshi_EditBox",
        pad, outputH + pad * 2,
        clientW - pad * 2, inputH,
        MyGUI::Align::Left | MyGUI::Align::Bottom, "KenshiPyInput");

    g_inputBox->setEditMultiLine(false);
    g_inputBox->setEditWordWrap(false);
    g_inputBox->eventEditSelectAccept += MyGUI::newDelegate(OnInputAccept);

    Logger::DebugLog("Console::Init complete");
}

void Console::Toggle()
{
    if (!g_consoleWindow)
        return;

    bool nowVisible = !g_consoleWindow->getVisible();
    g_consoleWindow->setVisible(nowVisible);

    if (nowVisible && g_inputBox)
    {
        MyGUI::InputManager* input = MyGUI::InputManager::getInstancePtr();
        input->setKeyFocusWidget(g_inputBox);
    }
}

static void ConsoleLog(const std::string& text)
{
    size_t end = text.size();
    while (end > 0 && (text[end - 1] == '\n' || text[end - 1] == '\r'))
        --end;
    if (end > 0)
        Logger::DebugLog(text.substr(0, end));
}

static void ConsoleError(const std::string& text)
{
    size_t end = text.size();
    while (end > 0 && (text[end - 1] == '\n' || text[end - 1] == '\r'))
        --end;
    if (end > 0)
        Logger::ErrorLog(text.substr(0, end));
}

void Console::AppendOutput(const std::string& text)
{
    // Mirror all console output to the KenshiPy_log.txt
    ConsoleLog(text);

    if (!g_outputBox || !g_outputScrollView)
        return;

    // Word-wrap at 100 characters so long lines don't run off screen.
    // Walks the incoming text inserting newlines at word boundaries where
    // possible, falling back to a hard break if no word boundary is found.
    const size_t WRAP = 100;
    std::string wrapped;
    wrapped.reserve(text.size());

    size_t lineLen = 0;
    size_t i = 0;
    while (i < text.size())
    {
        char c = text[i];

        if (c == '\n')
        {
            wrapped += c;
            lineLen = 0;
            ++i;
            continue;
        }

        if (lineLen < WRAP)
        {
            wrapped += c;
            ++lineLen;
            ++i;
            continue;
        }

        size_t lastSpace = wrapped.rfind(' ');
        if (lastSpace != std::string::npos && lastSpace >= wrapped.size() - WRAP)
        {
            wrapped[lastSpace] = '\n';
            lineLen = wrapped.size() - lastSpace - 1;
        }
        else
        {
            wrapped += '\n';
            lineLen = 0;
        }

        wrapped += c;
        ++lineLen;
        ++i;
    }

	// Append the wrapped text to the output box
    g_outputBox->setCaption(g_outputBox->getCaption() + MyGUI::UString(wrapped));

    int logSize = g_outputBox->getTextSize().height;
    int padding = g_outputBox->getTextRegion().top;
    int finalHeight = logSize + padding;
    int finalBottom = g_outputBox->getTop() + finalHeight;

    int viewHeight = g_outputScrollView->getViewCoord().height;
    if (finalBottom < viewHeight)
        finalBottom = viewHeight;

    g_outputBox->setSize(g_outputBox->getWidth(), finalBottom);

    if (g_outputScrollView->getCanvasSize().height < finalBottom)
    {
        g_outputScrollView->setCanvasSize(
            g_outputScrollView->getCanvasSize().width, finalBottom);
        g_outputScrollView->setVisibleHScroll(false);
    }

    g_outputScrollView->setViewOffset(MyGUI::IntPoint(
        0, -(finalBottom - g_outputScrollView->getViewCoord().height)));
}