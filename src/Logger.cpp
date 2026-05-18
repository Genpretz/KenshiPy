#include "Logger.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <cstdarg>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

// ---------------------------------------------------------------------------
// File-scope state
// ---------------------------------------------------------------------------

static std::ofstream       g_logFile;
static std::stringstream   g_logBuffer;
static boost::mutex        g_mutex;

// Each entry is one context name.  The back of the vector is the active one.
static std::vector<std::string> g_contextStack;

static DWORD g_startTick        = 0;
static bool  g_startInitialised = false;

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

static std::string CurrentTimestamp()
{
    if (!g_startInitialised)
    {
        g_startTick        = GetTickCount();
        g_startInitialised = true;
    }

    DWORD elapsed = GetTickCount() - g_startTick;
    char buf[32];
    sprintf_s(buf, "%lu.%03lu", elapsed / 1000, elapsed % 1000);
    return buf;
}

// Returns "[SomeMod/init.py] " or "" when the stack is empty.
static std::string CurrentContextPrefix()
{
    if (g_contextStack.empty())
        return "";
    return "[" + g_contextStack.back() + "] ";
}

enum Level
{
    Debug,
    Error
};

static void WriteLog(Level level, const char* message)
{
    // Caller must already hold g_mutex.
    const char* levelPrefix = (level == Level::Error) ? "ERROR: " : "";
    std::string ts          = CurrentTimestamp();
    std::string ctx         = CurrentContextPrefix();

    char line[4096];
    sprintf_s(line, "%s%s: %s%s", levelPrefix, ts.c_str(), ctx.c_str(), message);

    if (g_logFile.is_open())
    {
        g_logFile << line << '\n';
        // File streams are fully buffered; flush so logs appear on disk (and
        // survive crashes) instead of sitting in the stream buffer indefinitely.
        g_logFile.flush();
    }

    g_logBuffer << line << '\n';
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void InitLogger(const std::string& logFilePath)
{
    boost::lock_guard<boost::mutex> lock(g_mutex);
    if (!g_logFile.is_open())
        g_logFile.open(logFilePath.c_str());
}

namespace Logger
{
    void Debug(const char* fmt, ...)
    {
        boost::lock_guard<boost::mutex> lock(g_mutex);
        char buf[2048];
        va_list args;
        va_start(args, fmt);
        vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, args);
        va_end(args);
        WriteLog(Level::Debug, buf);
    }

    void Debug(const std::string& msg)
    {
        Debug("%s", msg.c_str());
    }

    void Error(const char* fmt, ...)
    {
        boost::lock_guard<boost::mutex> lock(g_mutex);
        char buf[2048];
        va_list args;
        va_start(args, fmt);
        vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, args);
        va_end(args);
        WriteLog(Level::Error, buf);
    }

    void Error(const std::string& msg)
    {
        Error("%s", msg.c_str());
    }

    void PushContext(const std::string& name)
    {
        boost::lock_guard<boost::mutex> lock(g_mutex);
        g_contextStack.push_back(name);
    }

    void PopContext()
    {
        boost::lock_guard<boost::mutex> lock(g_mutex);
        if (!g_contextStack.empty())
            g_contextStack.pop_back();
    }

    std::string GetLog()
    {
        boost::lock_guard<boost::mutex> lock(g_mutex);
        return g_logBuffer.str();
    }
}

void ForwardToLogger(const std::string& text)
{
    // Strip trailing whitespace before forwarding.
    size_t end = text.size();
    while (end > 0 && (text[end - 1] == '\n' || text[end - 1] == '\r' || text[end - 1] == ' '))
        --end;
    if (end > 0)
        Logger::Debug(text.substr(0, end));
}
