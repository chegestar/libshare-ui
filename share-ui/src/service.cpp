 
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
#include <MApplicationWindow>
#include <MComponentCache>
#include <ShareUI/ItemContainer>
#include <MSheet>
#include <QCoreApplication>
#include <QTimer>

Service::Service(QObject *parent) : QObject(parent) {
}

Service::~Service() {
    qDebug() << "Deleting the service";
}

void Service::share (const QStringList &fileList) {

    MApplicationWindow * window = MComponentCache::mApplicationWindow();

    qDebug() << "Created window " << window;

    ShareUI::ItemContainer * container = new ShareUI::ItemContainer (0, this);
    if (fileList.count() > 0) {
        container->appendItems (fileList);
    }
    
    ShareUI::PluginLoader *pLoader = new ShareUI::PluginLoader ();

        
    ShareWidgets::ApplicationViewInterface * iface =
        m_uiLoader.newDefaultApplicationView(pLoader, container);
    
    MSheet * page = dynamic_cast<MSheet*>(iface);

    
    if (page != 0) {
        connect (page, SIGNAL(shutdown()), this, SLOT(closeWindow()));
        connect (page, SIGNAL(closeButtonClicked()), this, SLOT(closeWindow()));
        if (pLoader->pluginCount() == 0) {
            pLoader->setPluginLoadingDelay(100);
            connect (page, SIGNAL(appeared()), pLoader, SLOT(loadPlugins()));
        }
        page->appear (window);
        window->show();
        connect (page, SIGNAL(destroyed()), pLoader, SLOT (deleteLater()));

    } else {
        delete pLoader;
        delete window;
        qCritical() << "Share failed: failed to load UI" << iface << page;
        QTimer::singleShot (500, this, SLOT (forceShutdownApp()));
    }
}

void Service::closeWindow() {
    MSheet * page = qobject_cast <MSheet*> (sender());

    if (page != 0) {
        qDebug() << "Close window called by:" << page;

        // Disconnect from signals emitted by this page first
        page->disconnect (this);
        page->deleteLater();        

        MApplicationWindow *activeWindow = page->applicationWindow();
        if(activeWindow != 0) {
            activeWindow->close();
            activeWindow->deleteLater();
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
