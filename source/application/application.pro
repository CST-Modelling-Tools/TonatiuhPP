TARGET = Tonatiuh
DESTDIR = ..

include(../config.pri)

QT += concurrent # for multithreading
QT += script # for scripting
QT += printsupport # for customplot
QT += gui # QVector3D

LIBS += -lTonatiuh-Kernel -lTonatiuh-Libraries -lOpengl32

HEADERS += \
    $$files(actions/*.h) \
    $$files(calculator/*.h) \
    $$files(commands/*.h) \
    $$files(run/*.h) \
    $$files(main/*.h) \
    $$files(parameters/*.h) \
    $$files(script/*.h) \
    $$files(tree/*.h) \
    $$files(view/*.h) \
    $$files(widgets/*.h)

SOURCES += \
    $$files(actions/*.cpp) \
    $$files(calculator/*.cpp) \
    $$files(commands/*.cpp) \
    $$files(run/*.cpp) \
    $$files(main/*.cpp) \
    $$files(parameters/*.cpp) \
    $$files(script/*.cpp) \
    $$files(tree/*.cpp) \
    $$files(view/*.cpp) \
    $$files(widgets/*.cpp)

FORMS += \
    $$files(calculator/*.ui) \
    $$files(run/*.ui) \
    $$files(main/*.ui) \
    $$files(script/*.ui) \
    $$files(widgets/*.ui)

RESOURCES += resources.qrc

win32 { # icon for windows explorer
    RC_ICONS = images/about/Tonatiuh.ico
}

DISTFILES += \
    images/run/runScript.png
