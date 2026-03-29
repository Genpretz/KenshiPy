#pragma once

#include <string>

namespace Console
{
    extern bool g_consoleInitialized;

    void Init();
    void Toggle();
    void AppendOutput(const std::string& text);
    void InitFrameHandler(float timeDelta);
    bool IsInitialized();
}