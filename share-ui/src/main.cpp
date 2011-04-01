 
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
#include "service.h"
#include "logger.h"
#include <QFileInfo>
#include "shareuiinterfaceadaptor.h"
#include <QDebug>
#include <stdlib.h>
#include <iostream>
#include <QCoreApplication>
#include <QApplication>

int main (int argc, char **argv) {

    bool run_standalone = true;
    
    QCoreApplication::setApplicationName (QLatin1String ("Share UI"));
    QCoreApplication::setOrganizationName (QLatin1String ("MeeGo"));    
    
    // Let's use custom logging
    Logger logger;    

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--service") == 0) {
            run_standalone = false;
        } else if (strcmp(argv[i], "--help") == 0) {
            std::cout << "Share UI command line usage is" << std::endl;
            std::cout << "\tshare-ui <file list>" << std::endl;
            exit(0);
        }
    }

    Service * service = new Service();
    QApplication * app = service->application (argc, argv);
    if (app == 0) {
        delete service;
        qCritical () << "Could not get QApplication pointer. Force quit";
        return -1;
    }

    if (run_standalone == true) {
        QStringList itemList;

        for (int i = 1; i < argc; i++) {
            if(argv[i][0] == '-') {
                // This is probably some kind of argument, like -software
                continue;
            }
            
            QString input = argv[i];
            
            //Simple input cleaner
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
        bool retA = connection.registerService(DBUS_SERVICE);
        bool retB = connection.registerObject("/", service);
        qDebug() << "Setup dbus connection" << retA << retB << DBUS_SERVICE;
    }
    
    int mainRes = app->exec();
    
    // As application is made by plugin inside service this order of deletion is important.
    delete app;
    delete service;
    
    qDebug() << "Clean shutdown of Share UI application";
    return mainRes;
}
