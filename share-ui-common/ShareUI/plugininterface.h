
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

#ifndef _SHARE_UI_PLUGIN_INTERFACE_H_ 
#define _SHARE_UI_PLUGIN_INTERFACE_H_

#include <ShareUI/export.h>
#include <QObject>
#include <QtPlugin>
#include <QList>

namespace ShareUI {

    class MethodBase;  

    /*!
       \class PluginInterface
       \brief Interface class for ShareUI plug-ins. PluginBase must be used
              as base class of plugins written.
       \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class SHARE_UI_EXPORT PluginInterface {

    public:
        /*!
          \brief  Virtual destructor
         */
        virtual ~PluginInterface()  {}
        
        /*!
          \brief Init function called when plugin is loaded
          \return <code>true</code> if plugin was initialized successfully.
                  <code>false</code> if plugin shouldn't be used after init
                  function.
         */
        virtual bool init() = 0;
        
        /*!
          \brief Ask for the method classes provided by this plugin. Called
                 only once between init and deinit. This function must
                 return list fast and must not block processing for long
                 time.
          \param parent Parent for methods loaded
          \return Method classes provided by plugin
         */
        virtual QList <MethodBase *> methods (QObject * parent = 0) = 0;
    };
}


Q_DECLARE_INTERFACE(ShareUI::PluginInterface,
    "com.nokia.shareUi.pluginInterface/1.0");

#endif // #ifndef _SHARE_UI_PLUGIN_INTERFACE_H_ 