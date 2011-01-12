QT += gui
TEMPLATE = app
CONFIG += qt debug meegotouch mdatauri
CONFIG += shareuiinterface-maemo-meegotouch

INCLUDEPATH += .
DEPENDPATH  += .
VPATH       += $$DEPENDPATH $$INCLUDEPATH

QMAKE_CXXFLAGS += -Werror -Wall

OBJECTS_DIR  = ./obj
OBJECTS_DIR  = ./moc

TARGET = link-share

SOURCES += main.cpp \
           page.cpp 

HEADERS += page.h
