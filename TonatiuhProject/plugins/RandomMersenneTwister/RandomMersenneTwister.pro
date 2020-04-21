TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug_and_release

include( ../../config.pri)

INCLUDEPATH += . \
    src \
    $$TONATIUH_ROOT/plugins \
    $$TONATIUH_ROOT/src


HEADERS = $$files(src/*.h)
SOURCES = $$files(src/*.cpp)



