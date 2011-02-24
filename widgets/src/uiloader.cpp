
/*
 * share-ui -- Handset UX Share user interface
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ShareWidgets/UiLoader"
#include "ShareWidgets/uiimplementationbase.h"
#include "uiloaderprivate.h"
#include <QFile>
#include <QDebug>
#include <QPluginLoader>

using namespace ShareWidgets;

UiLoader::UiLoader (QObject * parent) : QObject (parent),
    d_ptr (new UiLoaderPrivate (this)) {
}

UiLoader::~UiLoader () {
    delete d_ptr;
}

ApplicationViewInterface * UiLoader::newDefaultApplicationView (
    ShareUI::PluginLoader * pluginLoader, ShareUI::ItemContainer * container) {
    
    ApplicationViewInterface * retIface = 0;
        
    QString pluginPath = "/usr/lib/share-ui/implementations/libdefault.so";
    if (QFile::exists (pluginPath) == false) {
        qCritical() << "Failed to find default implementation for ShareUI:"
            << pluginPath;
        return 0;
    }
    
    QPluginLoader * loader = new QPluginLoader (this); 
    qDebug() << "Loading UI implementation from" << pluginPath;
    loader->setFileName (pluginPath);
    qDebug() << "Set loading hints for UIIloader";
    loader->setLoadHints (QLibrary::ExportExternalSymbolsHint);

    if (loader->load() == true) {
        
        ShareWidgets::UiImplementationBase * imple = 0;
        QObject * obj = loader->instance();
        
        if (obj != 0) {
            qDebug() << "Received object" << obj;
            imple = qobject_cast <ShareWidgets::UiImplementationBase*> (obj);
        } else {
            qWarning() << "Did not receive QObject instance from plugin";
        }
        
        if (imple != 0) {
            retIface = imple->newApplicationView (pluginLoader, container);
            retIface->setItemContainer (container);
            retIface->setPluginLoader (pluginLoader);
        } else {
            qWarning() << "Failed to cast to UiImplementation";
        }
    } else {
        qWarning() << "Failed to load plugin" << loader->errorString();
    }
    
    return retIface;
}

// --- private class -----------------------------------------------------------

UiLoaderPrivate::UiLoaderPrivate (UiLoader * parent) : m_uiLoader (parent) {
}

UiLoaderPrivate::~UiLoaderPrivate () {
}
