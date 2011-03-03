include (../common.pri)

CONFIG += qt debug link_pkgconfig qtsparql qtsparql-tracker-extensions
TEMPLATE = lib

# profiling
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

DEFINES += SUI_EXPORTS
QMAKE_CXXFLAGS += -O2 -Werror -Wall

#define descriptions in pluginloader.cpp file
#TODO: is there better way to do this with qmake?
DEFINES+="SHARE_UI_PLUGIN_FOLDER=\\\"$$[QT_INSTALL_LIBS]/share-ui/plugins\\\""

VER_MAJ=0
VER_MIN=2
VER_PAT=0

TARGET = share-ui-common

DEPENDPATH  += ./src
# ./src is where the private files will be kept
INCLUDEPATH += . \
               ./src 

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
QMAKE_CLEAN += obj/* \
               out/* \
               moc/*

INCLUDEPATH += ../mdatauri/inc
LIBS += ../mdatauri/out/libmdatauri.so

HEADERS += ShareUI/pluginbase.h \
           ShareUI/methodbase.h \
           ShareUI/item.h \
           ShareUI/itemcontainer.h \
           ShareUI/fileitem.h \
           ShareUI/datauriitem.h \
           ShareUI/pluginloader.h \
           itemcontainer_p.h \
           fileitem_p.h \
           pluginloader_p.h \
    src/pluginloaderthread.h \
    ShareUI/plugininterface.h
             
SOURCES += pluginbase.cpp \
           methodbase.cpp \
           item.cpp \
           itemcontainer.cpp \
           fileitem.cpp \
           datauriitem.cpp \
           pluginloader.cpp \
    src/pluginloaderthread.cpp
                
# Install binary application
target.path = $$INSTALL_LIB

# Install public headers
pubheaders.path = $$INSTALL_INC/ShareUI
pubheaders.files = ShareUI/*
INSTALLS += pubheaders

# Install old public headers TODO: REMOVE SOON
#oldheaders.path = $$INSTALL_INC/share-ui
#oldheaders.files = inc/*
#INSTALLS += oldheaders

# Install prf file for others to use
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = share-ui-common.prf \
            share-ui-plugin.prf
INSTALLS += prf
            
# Create pkgconfig file
CONFIG += create_pc create_prl
QMAKE_PKGCONFIG_REQUIRES = mdatauri
QMAKE_PKGCONFIG_NAME = share-ui-plugin
QMAKE_PKGCONFIG_DESCRIPTION = "Libraries for ShareUI plugins"
QMAKE_PKGCONFIG_LIBDIR = $$INSTALL_LIB
QMAKE_PKGCONFIG_INCDIR = $$INSTALL_INC
QMAKE_PKGCONFIG_CFLAGS = -DDBUS_SERVICE=\\\"$$DBUS_SERVICE\\\"
QMAKE_PKGCONFIG_DESTDIR = pkgconfig

# Install pkgconfig file for other to use
pkgconfig.path = $$INSTALL_LIB/pkgconfig
pkgconfig.target = share-ui-plugin.pc
pkgconfig.files = out/$$[QMAKE_PKGCONFIG_DESTDIR]/share-ui-common.pc \
                  share-ui-plugin.pc
pkgconfig.CONFIG     += no_check_exist
INSTALLS += pkgconfig

INSTALLS += target

# API Documentation with Doxygen
dox.path        = $$INSTALL_DOC/share-ui
!contains( DEFINES, NO_DOCS ) {
    dox.commands    = doxygen doxygen.cfg
}
dox.depends     = ShareUI
dox.files       = doc/html/*
dox.CONFIG     += no_check_exist
INSTALLS       += dox
QMAKE_EXTRA_TARGETS += dox
