#pragma once

#include "kernel/TonatiuhKernel.h"

#define NAME_ICON_FUNCTIONS(NAME, ICON) \
static const char* getClassName() {return NAME;} \
static const char* getClassIcon() {return ICON;} \
virtual const char* getTypeName() const {return getClassName();} \
virtual const char* getTypeIcon() const {return getClassIcon();}
