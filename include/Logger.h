#pragma once

#include <string>
#include <OgreCommon.h>


//----------------------------------------------------
// printf-style API (core logger)
//----------------------------------------------------
void DebugLog1(const char* fmt, ...);
void ErrorLog1(const char* fmt, ...);
//void ConsoleLog(const char* fmt, ...);

//----------------------------------------------------
// std::string overloads
//----------------------------------------------------

void DebugLog1(const std::string& msg);
void ErrorLog1(const std::string& msg);
//	void ConsoleLog(const std::string& msg);
//----------------------------------------------------
// Ogre::String overloads
//----------------------------------------------------
//void DebugLog(const Ogre::String& msg);
//void ErrorLog(const Ogre::String& msg);
//void ConsoleLog(const Ogre::String& msg);

//----------------------------------------------------
// Accessor
//----------------------------------------------------
std::string GetLog();