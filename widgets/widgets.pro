include(../common.pri)

CONFIG += qt debug link_pkgconfig 
TEMPLATE = lib
#PKGCONFIG +=

LIBS += ../share-ui-common/out/libshare-ui-common.so

# profiling
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

DEFINES += SHARE_WIDGETS_EXPORTS
QMAKE_CXXFLAGS += -O2 -Werror -Wall

VER_MAJ=0
VER_MIN=2
VER_PAT=0

TARGET = share-widgets

DEPENDPATH  += ./src

INCLUDEPATH += ./ShareWidgets \
               ../share-ui-common \
               ./src 

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
QMAKE_CLEAN += obj/* \
               out/* \
               moc/*
                
include (widgets-sources.pri)
                
# Install binary application
target.path = $$INSTALL_LIB

# Install public headers
pubheaders.path = $$INSTALL_INC/ShareWidgets
pubheaders.files = ShareWidgets/*
INSTALLS += pubheaders

# Install prf file for others to use
prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
prf.files = share-widgets.prf
INSTALLS += prf
            
# Install pkgconfig file for other to use
pkgconfig.path = $$INSTALL_LIB/pkgconfig
pkgconfig.files = share-widgets.pc
INSTALLS += pkgconfig

INSTALLS += target


