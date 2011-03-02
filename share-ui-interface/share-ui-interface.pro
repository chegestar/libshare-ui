include (../common.pri)
                
CONFIG += qt debug link_pkgconfig 
TEMPLATE = lib
TARGET = share-ui-interface
#PKGCONFIG +=

VER_MAJ=0
VER_MIN=1
VER_PAT=0

QMAKE_CXXFLAGS += -O2 -Werror -Wall

INCLUDEPATH += . ShareUI
VPATH += $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
QMAKE_CLEAN += $$OBJECTS_DIR/* $$MOC_DIR/* $$DESTDIR/*
                
!exists(shareuiinterface.cpp) {
    system(qdbusxml2cpp -c ShareUiInterface -p shareuiinterface -N ../share-ui/com.meego.ShareUiInterface.xml)
    system(sed -i \'$ i $${LITERAL_HASH}define SHAREUI_DBUS_SERVICE \"$${DBUS_SERVICE}\"\' shareuiinterface.h)
    system(mv -f shareuiinterface.h ShareUI/)
}
QMAKE_CLEAN += ShareUI/shareuiinterface.h shareuiinterface.cpp

SOURCES += shareuiinterface.cpp
HEADERS += ShareUI/shareuiinterface.h \
           ShareUI/ShareUiInterface

# Install public headers
pubheaders.path = $$INSTALL_INC/ShareUI
pubheaders.files = shareuiinterface.h
INSTALLS += pubheaders

# Install pkgconfig file for other to use
pkgconfig.path = $$INSTALL_LIB/pkgconfig/
pkgconfig.files = share-ui-interface.pc
INSTALLS += pkgconfig

# Install library
target.path = $$INSTALL_LIB
INSTALLS += target
