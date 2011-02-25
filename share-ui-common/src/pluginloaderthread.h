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

#ifndef _PLUGINLOADERTHREAD_H_
#define _PLUGINLOADERTHREAD_H_

#include <QThread>
#include <QStringList>
#include <QMutex>

class QPluginLoader;

namespace ShareUI {

/*!
   \class PluginLoaderThread
   \brief Thread for loading plugins

   \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
 */
class PluginLoaderThread : public QThread
{
    Q_OBJECT
public:
    PluginLoaderThread(const QStringList &queue, QThread *targetThread,
        QObject *parent = 0);

    virtual void run();

    void abort();

signals:

    void pluginLoaded(QPluginLoader *loader, bool last);

public slots:

private:

    QStringList m_queue; //!< Plugins not yet loaded

    QThread *m_targetThread; //!< Thread where loaded plugins are moved

    QMutex m_mutex;

    bool m_abort;

};

}

#endif
