#pragma once

#include <QtCore/qglobal.h>

#if defined(TONATIUH_LIBRARIES_EXPORT)
    #define TONATIUH_LIBRARIES Q_DECL_EXPORT
#else
    #define TONATIUH_LIBRARIES Q_DECL_IMPORT
#endif
