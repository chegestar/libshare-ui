######################################################################
# Automatically generated by qmake (2.01a) Tue Jul 28 10:16:12 2009
######################################################################

TEMPLATE = app
TARGET = 

INCLUDEPATH += . ../../share-ui/ ../../share-ui-interface
DEPENDPATH += .

CONFIG += shareuiinterface-maemo-meegotouch debug qt

QMAKE_CXXFLAGS += -Werror -Wall
LIBS += ../../share-ui-interface/out/libshare-ui-interface.so

OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
QMAKE_CLEAN += obj/* \
               moc/*

SOURCES += Share-UI-Example.cpp
