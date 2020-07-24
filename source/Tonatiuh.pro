TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += libraries
SUBDIRS += kernel
SUBDIRS += application
SUBDIRS += plugins
#SUBDIRS += tests
#SUBDIRS += installer

HEADERS += \
    application/script/SceneCreator.h

SOURCES += \
    application/script/SceneCreator.cpp
