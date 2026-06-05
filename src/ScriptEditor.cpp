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
#include "mygui/MyGUI_TextBox.h"
#include "mygui/MyGUI_Window.h"

// ---------------------------------------------------------------------------
// Module-level state
// ---------------------------------------------------------------------------

static bool g_initialized = false;

static MyGUI::Window*  g_window    = nullptr;
static MyGUI::EditBox* g_codePane  = nullptr;   // editable code
static MyGUI::EditBox* g_outputBox = nullptr;   // read-only output text

static std::string g_currentFilePath;

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

static void OnSaveAsClicked(MyGUI::Widget* sender)
{
	// Clearing stored file path forces SaveFileDialog to appear, even if the script was previously saved.
    g_currentFilePath.clear();
    OnSaveClicked(sender);
}

static void OnClearClicked(MyGUI::Widget* /*sender*/)
{
    if (g_codePane)
    {
        g_codePane->setCaption("");
    }
    g_currentFilePath.clear();

    ScriptEditor::ClearOutput();
}

static void OnWindowButton(MyGUI::Window* sender, const std::string& name)
{
    if (name == "close")
    {
        sender->setVisible(false);
    }
}

// ---------------------------------------------------------------------------
// ScriptEditor API
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

    const int W       = 800;
    const int H       = 600;
    const int PAD     = 4;
    const int BTN_H   = 30;
    const int BTN_W   = 80;
    const int OUT_H   = 200;

    g_window = gui->createWidget<MyGUI::Window>(
        "Kenshi_WindowCX",
        50, 50, W, H,
        MyGUI::Align::Default,
        "Window",
        "KenshiPyEditor");

    g_window->setCaption("KenshiPy Script Editor");
    g_window->setVisible(false);
    g_window->eventWindowButtonPressed += MyGUI::newDelegate(OnWindowButton);

    MyGUI::Widget* client  = g_window->getClientWidget();
    const int clientW      = client->getWidth();
    const int clientH      = client->getHeight();

    // -----------------------------------------------------------------------
    // Toolbar
    // -----------------------------------------------------------------------
    int btnX = PAD;
    int btnY = PAD;

    struct ButtonFactory
    {
        static MyGUI::Button* Create(
            MyGUI::Widget* client,
            int& btnX,
            int btnY,
            int BTN_W,
            int BTN_H,
            int PAD,
            const std::string& caption,
            const std::string& name,
            MyGUI::delegates::CDelegate1<MyGUI::Widget*>::IDelegate* handler)
        {
            MyGUI::Button* b = client->createWidget<MyGUI::Button>(
                "Kenshi_Button1",
                btnX, btnY, BTN_W, BTN_H,
                MyGUI::Align::Left | MyGUI::Align::Top,
                name);

            b->setCaption(caption);
            b->eventMouseButtonClick += handler;

            btnX += BTN_W + PAD;
            return b;
        }
    };

    ButtonFactory::Create(client, btnX, btnY, BTN_W, BTN_H, PAD, "Run", "RunButton", MyGUI::newDelegate(OnRunClicked));
    ButtonFactory::Create(client, btnX, btnY, BTN_W, BTN_H, PAD, "Open", "OpenButton", MyGUI::newDelegate(OnOpenClicked));
    ButtonFactory::Create(client, btnX, btnY, BTN_W, BTN_H, PAD, "Save", "SaveButton", MyGUI::newDelegate(OnSaveClicked));
    ButtonFactory::Create(client, btnX, btnY, BTN_W + 30, BTN_H, PAD, "Save As", "SaveAsButton", MyGUI::newDelegate(OnSaveAsClicked));
    ButtonFactory::Create(client, btnX, btnY, BTN_W, BTN_H, PAD, "Clear", "ClearButton", MyGUI::newDelegate(OnClearClicked));

    // -----------------------------------------------------------------------
    // Code editor
    // -----------------------------------------------------------------------

    // Load the skin XML that defines "Kenshi_ScriptEditor_InputEditBox".
    MyGUI::ResourceManager* res = MyGUI::ResourceManager::getInstancePtr();
    if (!res->load("ScriptEditor_EditBox.xml"))
        Logger::Error("ScriptEditor_EditBox.xml not found.");

    const int editorY = BTN_H + PAD * 2;
    const int editorH = clientH - editorY - OUT_H - PAD * 3;

    g_codePane = client->createWidget<MyGUI::EditBox>(
        "Kenshi_ScriptEditor_EditBox",
        PAD, editorY,
        clientW - PAD * 2,
        editorH,
        MyGUI::Align::Stretch,
        "KenshiPyCode");

    g_codePane->setEditMultiLine(true);
    g_codePane->setVisibleVScroll(true);
    g_codePane->setVisibleHScroll(true);
    g_codePane->setMaxTextLength(MyGUI::ITEM_NONE);

    // -----------------------------------------------------------------------
    // Output
    // -----------------------------------------------------------------------

    const int outputY = editorY + editorH + PAD;

    g_outputBox = client->createWidget<MyGUI::EditBox>(
        "Kenshi_ScriptEditor_EditBox",
        PAD, outputY,
        clientW - PAD * 2,
        OUT_H,
        MyGUI::Align::Stretch,
        "KenshiPyOutput");

    g_outputBox->setEditMultiLine(true);
    g_outputBox->setEditReadOnly(true);

    g_outputBox->setNeedMouseFocus(true);
    g_outputBox->setNeedKeyFocus(true);

    g_outputBox->setVisibleVScroll(true);
    g_outputBox->setVisibleHScroll(true);
    g_outputBox->setMaxTextLength(MyGUI::ITEM_NONE);

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
    ForwardToLogger(text);

    if (!g_outputBox)
        return;

    g_outputBox->addText(MyGUI::UString(text));

    size_t length = g_outputBox->getTextLength();
    g_outputBox->setTextCursor(length);
}

void ScriptEditor::ClearOutput()
{
    if (g_outputBox)
        g_outputBox->setCaption("");
}