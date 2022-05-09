#pragma once

#include <QtCore/qglobal.h>

//#define SUNPATHLIB

#ifdef SUNPATHLIB_EXPORT
    #define SUNPATHLIB Q_DECL_EXPORT
#else
    #define SUNPATHLIB Q_DECL_IMPORT
#endif
