\rm -rf \
    debian/files                \
    debian/tmp                  \
    debian/libshare-ui/         \
    debian/libshare-ui-dbg/     \
    debian/libshare-ui-dev/     \
    debian/libshare-ui-doc/     \
    debian/libshare-ui-utils/   \
    debian/libshare-ui-tests/   \
    debian/libshare-ui-I10-engineering-english/ \
    debian/*.substvars          \
    debian/*.debhelper

find . -name Makefile -exec \rm -rf {} \;
find . -name moc -exec \rm -rf {} \;
find . -name obj -exec \rm -rf {} \;
find . -name out -exec \rm -rf {} \;
find . -name *~ -exec \rm -rf {} \;

\rm -rf \
    build-stamp     \
    doc/            \
    config*         \
    share-ui.ts share-ui.qm

\rm -rf \
    ../libshare-ui*.deb     \
    ../libshare-ui*.dsc     \
    ../libshare-ui*.gz      \
    ../libshare-ui*.changes 

