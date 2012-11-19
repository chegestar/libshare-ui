include(../common.pri)

CONFIG += qt debug meegotouch
QT += testlib dbus
TEMPLATE = app

LIBS += ../share-ui-common/out/libshare-ui-common.so

TARGET = libshare-ui-tests

DEPENDPATH  += ./src \
               ../share-ui/src \
               ../share-ui-common/src \
               ../widgets/src \
               ../mdatauri/src \
               ../method-plugins/src
               
INCLUDEPATH += ../share-ui-common \
               ../share-ui-common/src \
               ../share-ui-common/ShareUI \
               ../share-ui/src \
               ../mdatauri/src \
               ../mdatauri/inc \
               ../widgets \
               ../widgets/src \
               ../widgets/ShareWidgets \
               ../method-plugins/src

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR     = ./moc
DESTDIR     = ./out

HEADERS += ShareUITests.h \
           mdatauri.h \
           bluetoothmethod.h \
           emailmethod.h \
           sharedmethoddata.h \
           pluginbase.h \
           defaultsplugin.h
            
SOURCES += ShareUITests.cpp \
           mdatauri.cpp \
           bluetoothmethod.cpp \
           emailmethod.cpp \
           methodscommon.cpp \
           sharedmethoddata.cpp \
           pluginbase.cpp \
           defaultsplugin.cpp
                        
CONFIG(profiling) {
    message(Building with profiling)
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    LIBS += -lgcov
}

# Hard-coded dependancy on libaccounts-glib as a workaround to this library not
# being able to be loaded multiple times
#LIBS += /usr/lib/libaccounts-glib.so.0

# This is needed to get unit test things included
QMAKE_CXXFLAGS += -DUNIT_TEST

target.path = /opt/tests/libshare-ui/

sh.path  = /opt/tests/libshare-ui/data/
sh.files = \
           image.jpg \
           tracker-copy.sh \
           tracker-clean.sh \
           run-test.sh

testdefinition.path = /opt/tests/libshare-ui/test-definition/
testdefinition.files = tests.xml
         
INSTALLS += target \
            sh \
            testdefinition
