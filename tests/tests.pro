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
               ../share-ui/src \
               ../mdatauri/src \
               ../mdatauri/inc \
               ../widgets \
               ../widgets/src \          
               ../method-plugins/src

VPATH += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR = ./obj
MOC_DIR     = ./moc
DESTDIR     = ./out

HEADERS += ShareUITests.h \
           mdatauri.h \
           bluetoothmethod.h \
           emailmethod.h
            
SOURCES += ShareUITests.cpp \
           mdatauri.cpp \
           bluetoothmethod.cpp \
           emailmethod.cpp \
           methodscommon.cpp      
                        
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

target.path = $$INSTALL_BIN

sh.path  = $$INSTALL_DATA/libshare-ui-tests
sh.files = tests.xml \
           image.jpg \
           tracker-copy.sh \
           tracker-clean.sh \
           run-test.sh
           
INSTALLS += target \
            sh \
