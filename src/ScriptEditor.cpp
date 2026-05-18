#include "ScriptEditor.h"
#include "PyRuntime.h"
#include "Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>

#include <Python.h>

#include <fstream>
#include <sstream>
#include <string>

#include "mygui/MyGUI_Button.h"
#include "mygui/MyGUI_EditBox.h"
#include "mygui/MyGUI_Gui.h"
#include "mygui/MyGUI_InputManager.h"
#include "mygui/MyGUI_ResourceManager.h"
#include "mygui/MyGUI_ScrollView.h"
#include "mygui/MyGUI_TextBox.h"
#include "mygui/MyGUI_Window.h"

// ---------------------------------------------------------------------------
// Module-level state
// ---------------------------------------------------------------------------

static bool g_initialized = false;

static MyGUI::Window*     g_window       = nullptr;
static MyGUI::EditBox*    g_codePane     = nullptr;   // editable code
static MyGUI::ScrollView* g_outputScroll = nullptr;   // output scroll container
static MyGUI::EditBox*    g_outputBox    = nullptr;   // read-only output text

static std::string g_currentFilePath;

// ---------------------------------------------------------------------------
// Output pane helpers
//
// Both AppendOutput and ClearOutput operate on g_outputScroll / g_outputBox.
// Extracted so the logic lives in exactly one place.
// ---------------------------------------------------------------------------

static void ScrollToBottom()
{
    if (!g_outputBox || !g_outputScroll)
        return;

    int textHeight  = g_outputBox->getTextSize().height;
    int topPad      = g_outputBox->getTextRegion().top;
    int totalHeight = textHeight + topPad;

    // Ensure the canvas is at least as tall as the text.
    int viewHeight = g_outputScroll->getViewCoord().height;
    if (totalHeight < viewHeight)
        totalHeight = viewHeight;

    g_outputBox->setSize(g_outputBox->getWidth(), totalHeight);

    if (g_outputScroll->getCanvasSize().height < totalHeight)
    {
        g_outputScroll->setCanvasSize(g_outputScroll->getCanvasSize().width, totalHeight);
        g_outputScroll->setVisibleHScroll(false);
    }

    g_outputScroll->setViewOffset(
        MyGUI::IntPoint(0, -(totalHeight - g_outputScroll->getViewCoord().height)));
}

// ---------------------------------------------------------------------------
// File dialogs
// ---------------------------------------------------------------------------

static std::string OpenFileDialog()
{
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn       = {};
    ofn.lStructSize         = sizeof(ofn);
    ofn.lpstrFilter         = "Python Files (*.py)\0*.py\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile           = filename;
    ofn.nMaxFile            = MAX_PATH;
    ofn.Flags               = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt         = "py";
    ofn.lpstrTitle          = "Open Python Script";
    return GetOpenFileNameA(&ofn) ? std::string(filename) : "";
}

static std::string SaveFileDialog()
{
    char filename[MAX_PATH] = "";
    if (!g_currentFilePath.empty())
        strncpy_s(filename, g_currentFilePath.c_str(), MAX_PATH - 1);

    OPENFILENAMEA ofn = {};
    ofn.lStructSize   = sizeof(ofn);
    ofn.lpstrFilter   = "Python Files (*.py)\0*.py\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile     = filename;
    ofn.nMaxFile      = MAX_PATH;
    ofn.Flags         = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt   = "py";
    ofn.lpstrTitle    = "Save Python Script";
    return GetSaveFileNameA(&ofn) ? std::string(filename) : "";
}

// ---------------------------------------------------------------------------
// Button handlers
// ---------------------------------------------------------------------------

static void OnRunClicked(MyGUI::Widget* /*sender*/)
{
    if (!g_codePane)
        return;

    std::string code = g_codePane->getCaption().asUTF8();
    if (code.empty())
    {
        ScriptEditor::AppendOutput("(editor is empty)\n");
        return;
    }

    const std::string label =
        g_currentFilePath.empty() ? "<editor>" : g_currentFilePath;

    ScriptEditor::AppendOutput("=== Running: " + label + " ===\n");
    ExecutePython(code, label, Py_file_input);
    ScriptEditor::AppendOutput("=== Done ===\n\n");
}

static void OnOpenClicked(MyGUI::Widget* /*sender*/)
{
    std::string path = OpenFileDialog();
    if (path.empty())
        return;

    std::ifstream f(path, std::ios::binary);
    if (!f.is_open())
    {
        ScriptEditor::AppendOutput("Failed to open: " + path + "\n");
        return;
    }

    std::ostringstream ss;
    ss << f.rdbuf();

    g_codePane->setCaption(MyGUI::UString(ss.str()));
    g_currentFilePath = path;
    ScriptEditor::AppendOutput("Loaded: " + path + "\n");
}

static void OnSaveClicked(MyGUI::Widget* /*sender*/)
{
    if (!g_codePane)
        return;

    std::string path = g_currentFilePath.empty() ? SaveFileDialog() : g_currentFilePath;
    if (path.empty())
        return;

    std::ofstream f(path, std::ios::binary);
    if (!f.is_open())
    {
        ScriptEditor::AppendOutput("Failed to save: " + path + "\n");
        return;
    }

    f << g_codePane->getCaption().asUTF8();
    g_currentFilePath = path;
    ScriptEditor::AppendOutput("Saved: " + path + "\n");
}

static void OnClearClicked(MyGUI::Widget* /*sender*/)
{
    if (g_codePane)
        g_codePane->setCaption("");
    g_currentFilePath.clear();
    ScriptEditor::ClearOutput();
}

static void OnWindowButton(MyGUI::Window* sender, const std::string& name)
{
    if (name == "close")
        sender->setVisible(false);
}

// ---------------------------------------------------------------------------
// ScriptEditor public API
// ---------------------------------------------------------------------------

void ScriptEditor::InitFrameHandler(float /*timeDelta*/)
{
    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
    if (!gui)
        return;

    // Wait until at least one widget exists — proxy for "GUI is ready".
    if (!gui->getEnumerator().next())
        return;

    gui->eventFrameStart -= MyGUI::newDelegate(ScriptEditor::InitFrameHandler);
    g_initialized = true;
    ScriptEditor::Init();
}

bool ScriptEditor::IsInitialized()
{
    return g_initialized;
}

void ScriptEditor::Init()
{
    // Register ourselves as the Python output sink so that print() and
    // tracebacks appear in our output pane.  PyRuntime owns the actual
    // sys.stdout/stderr redirect — we just set the destination.
    SetPythonOutputSink([](const std::string& text)
    {
        ScriptEditor::AppendOutput(text);
    });

    // -----------------------------------------------------------------------
    // Build the window
    // -----------------------------------------------------------------------
    MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();

    const int W       = 1000;
    const int H       = 700;
    const int PAD     = 4;
    const int BTN_H   = 30;
    const int BTN_W   = 80;
    const int OUT_H   = 200;   // height of the output pane

    g_window = gui->createWidget<MyGUI::Window>(
        "Kenshi_WindowCX",
        50, 50, W, H,
        MyGUI::Align::Default, "Window", "KenshiPyEditor");

    g_window->setCaption("KenshiPy Script Editor");
    g_window->setVisible(false);
    g_window->eventWindowButtonPressed += MyGUI::newDelegate(OnWindowButton);

    MyGUI::Widget* client  = g_window->getClientWidget();
    const int clientW      = client->getWidth();
    const int clientH      = client->getHeight();

    // --- Toolbar ---
    int btnX = PAD;
    int btnY = PAD;

    auto makeButton = [&](const std::string& caption,
        const std::string& name,
        MyGUI::delegates::CDelegate1<MyGUI::Widget*>::IDelegate* handler)
        -> MyGUI::Button*
        {
            MyGUI::Button* b = client->createWidget<MyGUI::Button>(
                "Kenshi_Button1",
                btnX, btnY, BTN_W, BTN_H,
                MyGUI::Align::Left | MyGUI::Align::Top, name);

            b->setCaption(caption);
            b->eventMouseButtonClick += handler;

            btnX += BTN_W + PAD;
            return b;
        };

    makeButton("Run",   "RunButton",   MyGUI::newDelegate(OnRunClicked));
    makeButton("Open",  "OpenButton",  MyGUI::newDelegate(OnOpenClicked));
    makeButton("Save",  "SaveButton",  MyGUI::newDelegate(OnSaveClicked));
    makeButton("Clear", "ClearButton", MyGUI::newDelegate(OnClearClicked));

    // --- Code editor ---
    // Load the skin XML that defines "Kenshi_ScriptEditor".
    MyGUI::ResourceManager* res = MyGUI::ResourceManager::getInstancePtr();
    if (!res->load("ScriptEditor.xml"))
        Logger::Error("ScriptEditor.xml not found — editor skin missing.");

    const int editorY = BTN_H + PAD * 2;
    const int editorH = clientH - editorY - OUT_H - PAD * 3;

    g_codePane = client->createWidget<MyGUI::EditBox>(
        "Kenshi_ScriptEditor",
        PAD, editorY,
        clientW - PAD * 2, editorH,
        MyGUI::Align::Stretch, "KenshiPyCode");

    g_codePane->setEditMultiLine(true);
    g_codePane->setEditWordWrap(false);
    g_codePane->setVisibleVScroll(true);
    g_codePane->setVisibleHScroll(true);

    // --- Output pane ---
    const int outputY = editorY + editorH + PAD;

    g_outputScroll = client->createWidget<MyGUI::ScrollView>(
        "Kenshi_ScrollViewEmpty",
        MyGUI::IntCoord(PAD, outputY, clientW - PAD * 2, OUT_H),
        MyGUI::Align::HStretch | MyGUI::Align::Bottom, "KenshiPyOutputScroll");

    g_outputScroll->setCanvasSize(
        g_outputScroll->getWidth() - 20,
        g_outputScroll->getHeight() - 20);

    g_outputBox = g_outputScroll->createWidget<MyGUI::EditBox>(
        "Kenshi_GenericTextBox",
        0, 0,
        g_outputScroll->getCanvasSize().width,
        g_outputScroll->getCanvasSize().height,
        MyGUI::Align::Stretch, "KenshiPyOutput");

    g_outputBox->setEditReadOnly(true);
    g_outputBox->setEditMultiLine(true);

    Logger::Debug("ScriptEditor initialised.");
}

void ScriptEditor::Toggle()
{
    if (!g_window)
        return;

    bool visible = !g_window->getVisible();
    g_window->setVisible(visible);

    if (visible && g_codePane)
        MyGUI::InputManager::getInstance().setKeyFocusWidget(g_codePane);
}

void ScriptEditor::AppendOutput(const std::string& text)
{
    // Always mirror to the log file so nothing is lost even when the window
    // is closed.
    ForwardToLogger(text);

    if (!g_outputBox || !g_outputScroll)
        return;

    std::string wrapped = text;
    g_outputBox->setCaption(g_outputBox->getCaption() + MyGUI::UString(wrapped));
    ScrollToBottom();
}

void ScriptEditor::ClearOutput()
{
    if (g_outputBox)
        g_outputBox->setCaption("");

    if (g_outputScroll)
        g_outputScroll->setViewOffset(MyGUI::IntPoint(0, 0));
}
