QT += gui
TEMPLATE = app
CONFIG += qt debug meegotouch mdatauri
CONFIG += shareuiinterface-maemo-meegotouch

INCLUDEPATH += . ../../share-ui/ ../../share-ui-interface ../../mdatauri/inc
DEPENDPATH  += .
VPATH       += $$DEPENDPATH $$INCLUDEPATH

QMAKE_CXXFLAGS += -Werror -Wall
LIBS += ../../share-ui-interface/out/libshare-ui-interface.so  ../../mdatauri/out/libmdatauri.so

OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
QMAKE_CLEAN += obj/* \
               moc/*

TARGET = link-share

SOURCES += main.cpp \
           page.cpp

HEADERS += page.h
