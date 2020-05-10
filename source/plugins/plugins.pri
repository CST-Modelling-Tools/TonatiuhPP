TEMPLATE = lib
DESTDIR = ..

include(../config.pri)

LIBS += -L../../..
LIBS += -lTonatiuh-Kernel -lTonatiuh-Libraries
