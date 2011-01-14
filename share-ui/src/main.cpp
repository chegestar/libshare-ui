 
/*
 * This file is part of Handset UX Share user interface
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
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




#include <QObject>
#include <MApplication>
#include <MApplicationWindow>
#include <MAction>
#include <MSceneWindow>
#include "service.h"
#include "logger.h"
#include <QFileInfo>
#include "shareuiinterfaceadaptor.h"
#include <QDebug>
#include <stdlib.h>
#include <iostream>

#include <MLocale>
#include <MExport>
#include <MComponentCache>
#include <QCoreApplication>

M_EXPORT int main (int argc, char **argv) {

    bool run_standalone = true;
    
    QCoreApplication::setApplicationName (QLatin1String ("Share UI"));

    // Use dynamic memory control to make sure free is successfull before we
    // print out bye message.
    MApplication * app = MComponentCache::mApplication (argc, argv);

    // Load the translation catalog. The engineering english catalog is per
    // application/package, and gets loaded automatically. In the device, there
    // will be only one translation catalog for share-ui, webupload-engine and
    // transfer-ui, and we need to explicitly load it
    MLocale locale;
    locale.installTrCatalog ("transfer");
    MLocale::setDefault (locale);

    // Let's use custom logging
    Logger logger;

    Service * service = new Service();

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--service") == 0) {
            run_standalone = false;
        } else if (strcmp(argv[i], "--help") == 0) {
            std::cout << "Share UI command line usage is" << std::endl;
            std::cout << "\tshare-ui <file list>" << std::endl;
            exit(0);
        }
    }

    if (run_standalone == true) {
        QStringList itemList;

        for (int i = 1; i < argc; i++) {
            if(argv[i][0] == '-') {
                // This is probably some kind of argument, like -software
                continue;
            }
            
            QString input = argv[i];
            
            if (input.startsWith ("data:") == true) {
                qDebug() << "Received data URI" << input;
                itemList << input;
            } else {
                QFileInfo fileInfo (input);
                QString fileUri = fileInfo.canonicalFilePath();
                if (fileUri.isEmpty ()) {
                    qDebug() << "Received Tracker IRI (?)" << input;
                    itemList << input;
                } else {
                    fileUri.prepend("file://");
                    qDebug() << "Received file path uri" << fileUri;
                    itemList << fileUri;
                }
            }
        }

        service->share (itemList);
        
    } else {

        new ShareUiInterfaceAdaptor (service);

        QDBusConnection connection = QDBusConnection::sessionBus();
        bool retA = connection.registerService("com.nokia.ShareUi");
        bool retB = connection.registerObject("/", service);
        qDebug() << "Setup dbus connection" << retA << retB;
    }
    
    int mainRes = app->exec();  
    qDebug() << "app returned" << mainRes;
    
    delete service;
    delete app;
    
    qDebug() << "Clean shutdown of Share UI application";
    return mainRes;
}
