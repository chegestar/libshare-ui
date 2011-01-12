#!/bin/sh

if [ "$(/usr/bin/id -u)" != "0" ]; then
    PATH=$HOME
else
    PATH=/home/user
fi

# Create the destination path if it does not exist
/bin/mkdir -p $PATH/MyDocs/.images

# Copy the test files to a location where they can be found by tracker
/bin/cp /usr/share/libshare-ui-tests/image.jpg $PATH/MyDocs/.images/libshare-ui-test.jpg || /bin/echo Failed to copy file

/bin/echo Give some time for tracker to index...
/bin/sleep 300
