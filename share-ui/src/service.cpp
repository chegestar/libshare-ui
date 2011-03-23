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


#include "service.h"
#include <QDebug>
#include <ShareUI/ItemContainer>
#include <ShareUI/PluginLoader>
#include <QApplication>
#include <QTimer>
#include <ShareWidgets/UiLoader>

#ifdef NOKIA_IMPLEMENTATION
#include <MApplication>
#endif

Service::Service(QObject *parent) : QObject(parent) {
}

Service::~Service() {
    qDebug() << "Deleting the service";
}

QApplication * Service::application (int &argc, char **argv) {
    QApplication * app = 0;
    if (m_uiLoader.loadPlugin ()) {
        app =  m_uiLoader.application (argc, argv);
    } 
        
    return app;
}

void Service::share (const QStringList &fileList) {

    ShareUI::PluginLoader *pLoader = new ShareUI::PluginLoader (this);

    ShareUI::ItemContainer * container = new ShareUI::ItemContainer (0, this);
    if (fileList.count() > 0) {
        container->appendItems (fileList);
    }

    qDebug() << "Created new container" << container;
    
    if (!m_uiLoader.showUI (pLoader, container)) {
        qCritical() << "Share failed: failed to load UI for items:";
        for (int i = 0; i < fileList.size (); ++i) {
            qCritical() << "\t" << fileList.at (i);
        }
    }
}

void Service::forceShutdownApp () {
    QCoreApplication * app = QCoreApplication::instance ();
    
    if (app != 0) {
        qCritical() << "Force shutdown application";
        app->quit ();
    } else {
        qCritical() << "Failed to force shutdown application!";
    }
}

