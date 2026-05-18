#pragma once

#include <functional>
#include <string>

// ---------------------------------------------------------------------------
// PyRuntime
//
// Owns the embedded Python interpreter lifetime and provides helpers for
// executing code and loading mod scripts.
//
// Output routing
// --------------
// Python's sys.stdout and sys.stderr are redirected to a C++ sink so that
// print() and tracebacks appear in our UI instead of being lost.
//
// By default the sink forwards everything to Logger::Debug.
// ScriptEditor calls SetOutputSink() after its window is built so that
// output appears in the editor's output pane while the interpreter is
// running.  You can pass nullptr to revert to the default Logger sink.
// ---------------------------------------------------------------------------

typedef std::function<void(const std::string&)> PythonOutputSink;
// Interpreter lifecycle.
void InitPython();
void ShutdownPython();

// Call once the GameWorld is ready; loads KenshiPy.json scripts from each
// active mod.
void TryLoadMods();

// Replace the current output sink.  Thread-safe.
// Pass an empty std::function (or nullptr-constructed one) to revert to
// the default Logger sink.
void SetPythonOutputSink(PythonOutputSink sink);

// Execute a string of Python code.
//   filename  — shown in tracebacks (use the real path or "<editor>")
//   startToken — Py_file_input (script), Py_single_input (REPL line)
// Returns true on success.
bool ExecutePython(const std::string& code,
                   const std::string& filename,
                   int                startToken);

// Load and execute a .py file from disk.
// Wraps ExecutePython with Py_file_input and Logger context tagging.
__declspec(dllexport) void RunScript(const std::string& scriptPath);

// Execute a string with Py_file_input (useful for scripted one-liners).
__declspec(dllexport) void RunString(const std::string& code);
