TEMPLATE = lib

include( ../config.pri)


INCLUDEPATH += $$TONATIUH_ROOT/plugin
LIBS += -L../..
DESTDIR = ../../plugins

LIBS += -lTonatiuhKernel
