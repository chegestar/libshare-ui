include (common.pri)

CONFIG += ordered
TEMPLATE        = subdirs
SUBDIRS        += mdatauri \
                  share-ui-common \
                  method-plugins \
                  widgets \                  
                  share-ui \
                  share-ui-interface \
                  tests \
                  translations \
                  examples

# Examples
pluginexample.path = /usr/share/doc/share-ui/plugin-example
pluginexample.files = method-plugins/plugin-example.pro \
                      method-plugins/src/emailmethod.cpp \
                      method-plugins/src/emailmethod.h \
                      method-plugins/src/bluetoothmethod.h \
                      method-plugins/src/bluetoothmethod.cpp \                    
                      method-plugins/src/smsmethod.cpp \
                      method-plugins/src/smsmethod.h \
                      method-plugins/src/defaultsplugin.h \
                      method-plugins/src/defaultsplugin.cpp
                      
datauriexample.path = /usr/share/doc/mdatauri/link-share-example
datauriexample.files = examples/link-share/link-share.pro \
                       examples/link-share/main.cpp \
                       examples/link-share/page.cpp \
                       examples/link-share/page.h

serviceexample.path  = /usr/share/doc/share-ui/service-example
serviceexample.files = examples/service-example/Share-UI-Example.cpp \
                        examples/service-example/service-example.pro

INSTALLS += pluginexample \
            datauriexample \
            serviceexample
