#!/bin/sh
# Clean up files copied for tracker

if [ "$(/usr/bin/id -u)" != "0" ]; then
    PATH=$HOME
else
    PATH=/home/user
fi

/bin/rm $PATH/MyDocs/.images/libshare-ui-test.jpg || /bin/echo Failed to remove libshare-ui-test.jpg
