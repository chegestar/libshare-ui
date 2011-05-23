
/*
 * This file is part of Handset UX Share user interface
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,     
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
 * IN THE SOFTWARE. 
 */

 
#include <ShareUI/ShareUiInterface>
#include <QStringList>
#include <QDebug>
#include <iostream>


int main(int argc, char **argv)
{
    // Create a QStringlist where file uri will be stored
    QStringList itemList;
    
    // Append the file uris to the list (files need to indexed by Tracker)
    // If application is reading files from Tracker then Tracker's "file ID
    // URIs" should be used instead. These are URIs of files entry in Tracker
    // database. In this example simple file path URI is used to keep code
    // simple.

    bool help = false;
    for(int i = 1; i < argc; ++i) {
        const QString arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            help = true;
            break;
        }

        if(arg.startsWith("-")) {
            // This is probably some kind of option, like -software.  Ignore.
            qDebug() << "Ignoring option" << arg;
            continue;
        }

        if (arg.startsWith ("data:") == true) {
            qDebug() << "Share data URI" << arg;
            itemList << arg;
        } else {
            QFileInfo fileInfo (arg);
            QString fileUri = fileInfo.canonicalFilePath();
            if (fileUri.isEmpty ()) {
                qDebug() << "Share Tracker IRI (?)" << arg;
                itemList << arg;
            } else {
                fileUri.prepend("file://");
                qDebug() << "Share file path uri" << fileUri;
                itemList << fileUri;
            }
        }
    }

    if (help || itemList.isEmpty()) {
        std::cout << "Usage: " << argv[0] << " { <data-uri> | <tracker-iri> | <file-uri> }+ "
                  << std::endl;
        exit(0);
    }

    // Create a interface object
    qDebug() << "Connecting to service" << SHAREUI_DBUS_SERVICE;
    ShareUiInterface shareIf(SHAREUI_DBUS_SERVICE, "/", QDBusConnection::sessionBus());
    
    // You can check if interface is valid
    if (shareIf.isValid()) {
        // Start ShareUI application with selected files. 
        qDebug() << "Signalling share-ui daemon...";
        shareIf.share (itemList);
    } else {
        qCritical() << "Invalid interface";
        return -1;
    }
}
