#pragma once

#ifdef __linux__
    //linux code goes here
#elif _WIN32

#include <windows.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#endif
