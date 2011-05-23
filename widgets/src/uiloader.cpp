
/*
 * share-ui -- Handset UX Share user interface
 * Copyright (c) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef SHARE_UI_IMPLEMENTATION_LIBRARY
// .so file providing the implementation of the share-ui 
#define SHARE_UI_IMPLEMENTATION_LIBRARY "/usr/lib/share-ui/implementations/libdefault.so"
#endif

using namespace ShareWidgets;

UiLoader::UiLoader (QObject * parent) : QObject (parent),
    d_ptr (new UiLoaderPrivate (this)) {
}

UiLoader::~UiLoader () {
    delete d_ptr;
}

bool UiLoader::loadPlugin (QString pPath) {

    bool pluginLoaded = false;

    if (d_ptr->m_impl != 0) {
        // Plugin has already been loaded once.
        return true;
    }

    QString pluginPath = pPath;
    if (pPath.isEmpty()) {
        pluginPath = SHARE_UI_IMPLEMENTATION_LIBRARY;
    }

    if (QFile::exists (pluginPath) == false) {
        qCritical() << "Failed to find default implementation for ShareUI:"
            << pluginPath;
    } else {
        QPluginLoader * loader = new QPluginLoader (this); 
        qDebug() << "Loading UI implementation from" << pluginPath;
        loader->setFileName (pluginPath);
        qDebug() << "Set loading hints for UIIloader";
        loader->setLoadHints (QLibrary::ExportExternalSymbolsHint);

        if (loader->load() == true) {
            
            QObject * obj = loader->instance();
            
            if (obj != 0) {
                qDebug() << "Received object" << obj;
                d_ptr->m_impl = 
                    qobject_cast <ShareWidgets::UiImplementationBase*> (obj);
                if (d_ptr->m_impl != 0) {
                    pluginLoaded = true;
                } else {
                    qWarning() << "Failed to cast to UiImplementation";
                }
            } else {
                qWarning() << "Did not receive QObject instance from plugin";
            }
        } else {
            qWarning() << "Failed to load plugin" << loader->errorString();
        }
    }

    return pluginLoaded;
}

QApplication * UiLoader::application (int & argc, char ** argv) {

    // Ask for new application if not already loaded
    if (d_ptr->m_application == 0) {
        if (d_ptr->m_impl != 0) {
            d_ptr->m_application = d_ptr->m_impl->application(argc, argv);
        }
    }

    return d_ptr->m_application;
}

bool UiLoader::showUI (ShareUI::PluginLoader * pluginLoader,
    ShareUI::ItemContainer * container) {

    if (d_ptr->m_impl != 0) {
        return d_ptr->m_impl->showUI (pluginLoader, container);
    } 

    return false;
} 


// --- private class -----------------------------------------------------------

UiLoaderPrivate::UiLoaderPrivate (UiLoader * parent) : m_uiLoader (parent),
    m_impl (0), m_application (0) {
}

UiLoaderPrivate::~UiLoaderPrivate () {
}
