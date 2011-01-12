TEMPLATE     = lib
CONFIG      += qt debug plugin meegotouch link_pkgconfig share-ui-common

TARGET = example-plugin

QMAKE_CXXFLAGS += -Werror

DEPENDPATH += ./src
INCLUDEPATH += ./src
VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR = ./moc
DESTDIR = ./out
           
HEADERS += emailmethod.h \
           bluetoothmethod.h \
           defaultsplugin.h

SOURCES += emailmethod.cpp \
           bluetoothmethod.cpp \
           defaultsplugin.cpp

target.path = /usr/lib/share-ui/plugins
INSTALLS += target
