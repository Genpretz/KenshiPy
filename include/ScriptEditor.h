#pragma once

#include <string>

// ---------------------------------------------------------------------------
// ScriptEditor
//
// A MyGUI window containing:
//   - A toolbar  (Run | Open | Save | Clear buttons)
//   - A code editor pane  (multi-line editable text)
//   - An output pane      (read-only, receives Python stdout/stderr)
//
// Keyboard shortcut:  Ctrl+`  toggles visibility.
//
// Lifecycle:
//   InitFrameHandler is registered on MyGUI::Gui::eventFrameStart by the
//   TitleScreen hook.  It waits until the GUI is ready, then calls Init()
//   exactly once.
// ---------------------------------------------------------------------------

namespace ScriptEditor
{
    // Called by the TitleScreen hook via MyGUI's frame-start event.
    // Waits for the GUI to be ready, then calls Init().
    void InitFrameHandler(float timeDelta);

    bool IsInitialized();

    // Build the MyGUI widgets.  Called once by InitFrameHandler.
    void Init();

    // Show / hide the editor window.
    void Toggle();

    // Append text to the output pane and scroll to the bottom.
    // Also forwards text to Logger::Debug so everything is in the log file.
    void AppendOutput(const std::string& text);

    // Clear the output pane.
    void ClearOutput();
}
