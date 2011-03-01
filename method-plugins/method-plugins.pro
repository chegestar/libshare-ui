include(../common.pri)

TEMPLATE     = lib
CONFIG      += qt debug plugin meegotouch link_pkgconfig  qdbus

TARGET = defaults

QMAKE_CXXFLAGS += -O2 -Werror -Wall

# profiling
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

DEPENDPATH += ./src
INCLUDEPATH += ./src \
               ../share-ui-common \
               ../mdatauri/inc
VPATH += $$DEPENDPATH $$INCLUDEPATH

LIBS += ../share-ui-common/out/libshare-ui-common.so

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
           
#including the pri file that has the sources list
include (method-plugins-source.pri)

target.path = $$INSTALL_LIB/share-ui/plugins
INSTALLS += target
