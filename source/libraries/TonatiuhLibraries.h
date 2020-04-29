#pragma once

#include <QtCore/qglobal.h>

#if defined(TONATIUH_LIBRARIES_EXPORT)
    #define TONATIUH_LIBRARIES Q_DECL_EXPORT
    #define QCUSTOMPLOT_COMPILE_LIBRARY
#else
    #define TONATIUH_LIBRARIES Q_DECL_IMPORT
    #define QCUSTOMPLOT_USE_LIBRARY
#endif
