#pragma once

#include <string>

namespace Logger
{
	//----------------------------------------------------
	// Initialization
	//----------------------------------------------------
	void InitLogger(const std::string& logFilePath);

	//----------------------------------------------------
	// printf-style API (core logger)
	//----------------------------------------------------
	void DebugLog(const char* fmt, ...);
	void ErrorLog(const char* fmt, ...);
	//	void ConsoleLog(const char* fmt, ...);

	//----------------------------------------------------
	// std::string overloads
	//----------------------------------------------------
	void DebugLog(const std::string& msg);
	void ErrorLog(const std::string& msg);
	//	void ConsoleLog(const std::string& msg);
}
//----------------------------------------------------
// Accessor
//----------------------------------------------------
std::string GetLog();