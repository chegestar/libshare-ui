LANGUAGES =        # Empty because we are only generating engineering english
CATALOGNAME = share-ui
SOURCEDIR = $$PWD/../share-ui/src \
            $$PWD/../method-plugins/src
TRANSLATEDIR = $$PWD

include ($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_defines.prf)
include ($$[QT_INSTALL_DATA]/mkspecs/features/meegotouch_translations.prf)

