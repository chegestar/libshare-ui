TEMPLATE = app
CONFIG += qt debug meegotouch link_pkgconfig
QT += dbus 

CONFIG += meegotouch-boostable

# profiling
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

QMAKE_CXXFLAGS += -O2 -Werror -Wall

LIBS += ../widgets/out/libshare-widgets.so
INCLUDEPATH += ../widgets

TARGET = share-ui

DEPENDPATH  += . ./src ../libsharewidgets/src
               
INCLUDEPATH += ../share-ui-common \
               ../libsharewidgets/src

CONFIG(meego-com) {
    system(qdbusxml2cpp -a shareuiinterfaceadaptor com.meego.ShareUiInterface)
    service.files = com.meego.ShareUi.service
    DEFINES += MEEGO_COM
} else {
# Interim migration path until m-servicefwgen starts using com.meego.ShareUiInterface
# Need to first copy the interface xml file, because m-servicefwgen currently
# works only on the current directory. Need to raise bug about this
    system(cp /usr/share/dbus-1/interfaces/com.nokia.maemo.meegotouch.ShareUiInterface.xml .)
    system(m-servicefwgen -a com.nokia.maemo.meegotouch.ShareUiInterface)
    service.files = com.nokia.ShareUi.service
}
VPATH       += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc
DESTDIR      = ./out
QMAKE_CLEAN += obj/* \
               out/* \
               moc/*           
             
#including the pri file that has the sources list
include (share-ui-source.pri)
SOURCES += shareuiinterfaceadaptor.cpp

QMAKE_CLEAN += com.nokia.maemo.meegotouch.ShareUiInterface.xml \
               shareuiinterfacedaptor.h \
               shareuiinterfaceadaptor.cpp

INCLUDEPATH += .
LIBS += ../share-ui-common/out/libshare-ui-common.so

INCLUDEPATH += ../mdatauri/MDataURI
LIBS += ../mdatauri/out/libmdatauri.so

service.path = /usr/share/dbus-1/services

#Install binary application
target.path = /usr/bin

INSTALLS += target        \
            service
