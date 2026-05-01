#include "Logger.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdarg>
#include <cstdio>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

enum LogLevel
{
    LOG_DEBUG,
    LOG_ERROR,
    LOG_CONSOLE
};

static std::ofstream debugFile;
static std::stringstream debugLog;
static DWORD startTime = 0;
static bool startTimeInitialized = false;
static boost::mutex logMutex;

namespace Logger
{
    void InitLogger(const std::string& logFilePath)
    {
        boost::lock_guard<boost::mutex> lock(logMutex);
        if (!debugFile.is_open())
        {
            debugFile.open(logFilePath.c_str());
        }
    }
}

static void LogImpl(LogLevel level, const char* fmt, va_list args)
{
    boost::lock_guard<boost::mutex> lock(logMutex);

    if (!startTimeInitialized)
    {
        startTime = GetTickCount();
        startTimeInitialized = true;
    }

    char messageBuffer[2048];
    vsnprintf_s(messageBuffer, sizeof(messageBuffer), _TRUNCATE, fmt, args);

    DWORD elapsed = GetTickCount() - startTime;
    DWORD seconds = elapsed / 1000;
    DWORD millis = elapsed % 1000;

    char timeStr[32];
    sprintf_s(timeStr, "%lu.%03lu", seconds, millis);

    const char* prefix = "";
    switch (level)
    {
    case LOG_DEBUG:   prefix = ""; break;
    case LOG_ERROR:   prefix = "Error "; break;
        //case LOG_CONSOLE: prefix = "Console "; break;
    }

    char finalBuffer[2300];
    sprintf_s(finalBuffer, "%s%s: %s", prefix, timeStr, messageBuffer);

    if (debugFile.is_open())
    {
        debugFile << finalBuffer << std::endl;
        if (level == LOG_ERROR)
            debugFile << std::flush;
    }

    debugLog << finalBuffer << std::endl;
}

namespace Logger
{
    void DebugLog(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogImpl(LOG_DEBUG, fmt, args);
        va_end(args);
    }

    void DebugLog(const std::string& msg)
    {
        DebugLog("%s", msg.c_str());
    }

    void ErrorLog(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        LogImpl(LOG_ERROR, fmt, args);
        va_end(args);
    }

    void ErrorLog(const std::string& msg)
    {
        ErrorLog("%s", msg.c_str());
    }

    //void ConsoleLog(const char* fmt, ...)
    //{
    //    va_list args;
    //    va_start(args, fmt);
    //    LogImpl(LOG_CONSOLE, fmt, args);
    //    va_end(args);
    //}

    //void ConsoleLog(const std::string& msg)
    //{
    //    ConsoleLog("%s", msg.c_str());
    //}
}

std::string GetLog()
{
    boost::lock_guard<boost::mutex> lock(logMutex);
    return debugLog.str();
}