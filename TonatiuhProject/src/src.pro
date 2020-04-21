include( ../config.pri )

TARGET = Tonatiuh   

QT += xml opengl svg script
QT += concurrent printsupport


HEADERS += \
$$files(source/application/*.h) \
$$files(source/auxiliary/*.h) \
$$files(source/geometry/*.h) \
$$files(source/gui/*.h) \
$$files(source/raytracing/*.h) \
$$files(source/statistics/*.h)

FORMS += $$files(source/gui/*.ui)

SOURCES += \
$$files(source/application/*.cpp) \
$$files(source/auxiliary/*.cpp) \
$$files(source/geometry/*.cpp) \
$$files(source/gui/*.cpp) \
$$files(source/raytracing/*.cpp) \
$$files(source/statistics/*.cpp)

RESOURCES += tonatiuh.qrc
 
DESTDIR = ../bin

win32 {
    RC_FILE = Tonatiuh.rc
}

LIBS += -L$$COINDIR/bin # to run

#install.files += $$files($$OUT_PWD/../bin/*.dll) )
#install.path = $$PWD/../../bin

#INSTALLS += install
