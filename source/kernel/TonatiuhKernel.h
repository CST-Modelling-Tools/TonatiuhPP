#pragma once

#include <QtCore/qglobal.h>

#if defined(TONATIUH_KERNEL_EXPORT)
    #define TONATIUH_KERNEL Q_DECL_EXPORT
#else
    #define TONATIUH_KERNEL Q_DECL_IMPORT
#endif
