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
DEFINES+="SHARE_UI_PLUGIN_FOLDER=\\\"$$[QT_INSTALL_LIBS]/share-ui/plugins\\\"

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
           pluginloader_p.h
             
SOURCES += pluginbase.cpp \
           methodbase.cpp \
           item.cpp \
           itemcontainer.cpp \
           fileitem.cpp \
           datauriitem.cpp \
           pluginloader.cpp
                
# Install binary application
target.path = /usr/lib

# Install public headers
pubheaders.path = /usr/include/ShareUI
pubheaders.files = ShareUI/*
INSTALLS += pubheaders

# Install old public headers TODO: REMOVE SOON
#oldheaders.path = /usr/include/share-ui
#oldheaders.files = inc/*
#INSTALLS += oldheaders

# Install prf file for others to use
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = share-ui-common.prf \
            share-ui-plugin.prf
INSTALLS += prf
            
# Install pkgconfig file for other to use
pkgconfig.path = /usr/lib/pkgconfig
pkgconfig.files = share-ui-plugin.pc
INSTALLS += pkgconfig

INSTALLS += target

# API Documentation with Doxygen
dox.path        = /usr/share/doc/share-ui
!contains( DEFINES, NO_DOCS ) {
    dox.commands    = doxygen doxygen.cfg
}
dox.depends     = ShareUI
dox.files       = doc/html/*
dox.CONFIG     += no_check_exist
INSTALLS       += dox
QMAKE_EXTRA_TARGETS += dox

