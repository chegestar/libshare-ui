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

#include "pluginloaderthread.h"
#include <QPluginLoader>
#include <QMutexLocker>
#include <QDebug>

using namespace ShareUI;

PluginLoaderThread::PluginLoaderThread(const QStringList &queue,
    QThread *targetThread, QObject *parent)
 :  QThread(parent),
    m_queue(queue),
    m_targetThread(targetThread),
    m_abort(false)
{
}

void PluginLoaderThread::run()
{
    if (m_queue.empty() || m_targetThread == 0) {
        qCritical() << "Invalid plugin loading parameters";
        return;
    }

    while (!m_queue.isEmpty()) {
        QPluginLoader * loader = new QPluginLoader ();
        loader->setFileName (m_queue.takeFirst());
        loader->setLoadHints (QLibrary::ExportExternalSymbolsHint);

        if (loader->load()) {
            QMutexLocker locker(&m_mutex);
            if (!m_abort) {
                loader->moveToThread(m_targetThread);
                //loader->setParent(parent());
            }
            else {
                qDebug() << "Plugin loading aborted, exiting loader thread";
                delete loader;
                loader = 0;
                break;
            }
        }
        else {
            qCritical() << "Failed to load plugin:" << loader->errorString();
            delete loader;
            loader = 0;
        }

        Q_EMIT(pluginLoaded(loader, m_queue.isEmpty()));
    }
}

void PluginLoaderThread::abort()
{
    QMutexLocker locker(&m_mutex);

    m_abort = true;
}
