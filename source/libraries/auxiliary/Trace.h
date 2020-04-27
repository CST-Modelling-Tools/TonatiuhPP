#pragma once

#include "libraries/TonatiuhLibraries.h"
#include <string>

class TONATIUH_LIBRARIES Trace
{
public:
    Trace(std::string functionName, bool showTrace = true);
    ~Trace();

private:
    static int level;
    static bool TraceEnabled;

    std::string m_functionName;
    bool m_functionTrace;
};
