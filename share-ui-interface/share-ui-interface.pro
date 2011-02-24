include (../common.pri)
                
CONFIG += qt debug link_pkgconfig 
TEMPLATE = lib
TARGET = share-ui-interface
#PKGCONFIG +=

VER_MAJ=0
VER_MIN=1
VER_PAT=0

QMAKE_CXXFLAGS += -O2 -Werror -Wall

INCLUDEPATH += .
VPATH += $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
QMAKE_CLEAN += $$OBJECTS_DIR/* $$MOC_DIR/* $$DESTDIR/*
                
!exists(shareuiinterface.h) {
    system(qdbusxml2cpp -c ShareUiInterface -p shareuiinterface -N ../share-ui/com.meego.ShareUiInterface.xml)
}
QMAKE_CLEAN += shareuiinterface.h shareuiinterface.cpp

SOURCES += shareuiinterface.cpp
HEADERS += shareuiinterface.h

# Install public headers
pubheaders.path = /usr/include/ShareUI
pubheaders.files = shareuiinterface.h
INSTALLS += pubheaders

# Install pkgconfig file for other to use
pkgconfig.path = /usr/lib/pkgconfig/
pkgconfig.files = share-ui-interface.pc
INSTALLS += pkgconfig

# Install library
target.path = /usr/lib/
INSTALLS += target
