CONFIG += meego-com

CONFIG(meego-com) {
    DBUS_ROOT = com.meego
    DEFINES += MEEGO_COM
} else {
    DBUS_ROOT = com.nokia
}

DBUS_SERVICE   = $${DBUS_ROOT}.ShareUi
DBUS_INTERFACE = $${DBUS_ROOT}.ShareUiInterface

DEFINES += DBUS_SERVICE=$$DBUS_SERVICE
