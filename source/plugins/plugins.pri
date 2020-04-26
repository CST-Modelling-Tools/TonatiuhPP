TEMPLATE = lib
DESTDIR = ..

include( ../config.pri)

LIBS += -L../../..
LIBS += -lTonatiuhKernel -lTonatiuhLibraries
