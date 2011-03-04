include (../common.pri)

TEMPLATE = app
CONFIG += qt link_pkgconfig
CONFIG += debug
QT += dbus 

# profiling
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

QMAKE_CXXFLAGS += -O2 -Werror -Wall -fPIC

LIBS += ../widgets/out/libshare-widgets.so
INCLUDEPATH += ../widgets

TARGET = share-ui

DEPENDPATH  += . ./src ../libsharewidgets/src
               
INCLUDEPATH += ../share-ui-common \
               ../libsharewidgets/src

CONFIG(meego-com) {
    system(qdbusxml2cpp -a shareuiinterfaceadaptor $${DBUS_INTERFACE}.xml)
} else {
    # Interim migration path until m-servicefwgen starts using
    # com.meego.ShareUiInterface. Need to first copy the interface xml file,
    # because m-servicefwgen currently works only on the current directory.
    # Need to raise bug about this

    system(cp /usr/share/dbus-1/interfaces/com.nokia.maemo.meegotouch.ShareUiInterface.xml \
              $${DBUS_INTERFACE}.xml)
    system(m-servicefwgen -a $$DBUS_INTERFACE)
    QMAKE_CLEAN += $${DBUS_INTERFACE}.xml
    
    #Nokia implementation needs MeegoTouch libraries to initialize MApplication
    CONFIG += meegotouch
    
    DEFINES += NOKIA_IMPLEMENTATION
}

SOURCES     += shareuiinterfaceadaptor.cpp
HEADERS     += shareuiinterfaceadaptor.h
QMAKE_CLEAN += shareuiinterfaceadaptor.h \
               shareuiinterfaceadaptor.cpp


VPATH       += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
DESTDIR      = ./out
QMAKE_CLEAN += $$OBJECTS_DIR/* \
               $$MOC_DIR/*     \
               $$DESTDIR/*
             
#including the pri file that has the sources list
include (share-ui-source.pri)

INCLUDEPATH += .
LIBS += ../share-ui-common/out/libshare-ui-common.so
LIBS += -rdynamic

INCLUDEPATH += ../mdatauri/MDataURI
LIBS += ../mdatauri/out/libmdatauri.so

service.files = $${DBUS_SERVICE}.service
service.path = $$INSTALL_DATA/dbus-1/services

#Install binary application
target.path = $$INSTALL_BIN

INSTALLS += target        \
            service
