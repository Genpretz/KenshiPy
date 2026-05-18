#pragma once

#include <string>

// ---------------------------------------------------------------------------
// Logger
//
// Thread-safe file + in-memory logger with an optional context stack.
// Push a context (e.g. a script filename) before running code so that every
// log line emitted during that block is prefixed with [context].
//
// Usage:
//   Logger::PushContext("MyMod/init.py");
//   // ... run script ...
//   Logger::PopContext();
//
// Or use the RAII helper:
//   {
//       Logger::ScopedContext ctx("MyMod/init.py");
//       RunScript(...);
//   }  // context popped automatically
// ---------------------------------------------------------------------------

void InitLogger(const std::string& logFilePath);

namespace Logger
{
    // -----------------------------------------------------------------------
    // Core log functions.  All are safe to call from any thread.
    // -----------------------------------------------------------------------
    void Debug  (const char* fmt, ...);
    void Error  (const char* fmt, ...);

    void Debug  (const std::string& msg);
    void Error  (const std::string& msg);

    // -----------------------------------------------------------------------
    // Context stack — affects the prefix of every line logged while active.
    // Contexts nest; only the innermost one is shown.
    // -----------------------------------------------------------------------
    void PushContext(const std::string& name);
    void PopContext();

    // RAII wrapper — push on construction, pop on destruction.
    struct ScopedContext
    {
        explicit ScopedContext(const std::string& name)
        {
            PushContext(name);
        }

        ~ScopedContext()
        {
            PopContext();
        }

    private:
        // Non-copyable, non-movable (C++03 style)
        ScopedContext(const ScopedContext&);
        ScopedContext& operator=(const ScopedContext&);
    };

    // -----------------------------------------------------------------------
    // Retrieve everything logged so far as a single string (for diagnostics).
    // -----------------------------------------------------------------------
    std::string GetLog();
}

// Convenience: strip trailing newlines then send to Logger::Debug.
// Used when forwarding Kenshi's own log callbacks.
void ForwardToLogger(const std::string& text);
