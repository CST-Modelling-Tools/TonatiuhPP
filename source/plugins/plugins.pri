TEMPLATE = lib
DESTDIR = ..

include( ../config.pri)

#INCLUDEPATH += $$TONATIUH_ROOT/plugin
LIBS += -L../../..
LIBS += -lTonatiuhKernel
