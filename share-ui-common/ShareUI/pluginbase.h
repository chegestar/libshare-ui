
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
 
#ifndef _SHARE_UI_PLUGIN_BASE_H_ 
#define _SHARE_UI_PLUGIN_BASE_H_

#include <ShareUI/export.h>
#include <QObject>
#include <ShareUI/PluginInterface>

namespace ShareUI {

    class MethodBase;  

    /*!
       \class PluginBase
       \brief Adds QObject to pure virtual interface class and gives default
              implementation for some optional functions.
       \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class SHARE_UI_EXPORT PluginBase : public QObject, PluginInterface {

    Q_OBJECT
    Q_INTERFACES (ShareUI::PluginInterface)

    public:
    
        /*!
          \brief Constructor
          \param parent QObject parent
         */
        PluginBase (QObject * parent = 0);
        
        virtual ~PluginBase();
        
        /*!
          \brief Default implementation for init that does nothing.
          \return Default implementation always returns <code>true</code>. If
                  reimplementation returns false then plugin is ignored and
                  methods function is not called.
         */
        virtual bool init();
        
        /*!
          \brief This function returns ShareUI::MethodBase pointers as methods
                 to be presented to user as sharing option.
          \param parent Parent for MethodBase classes returned. Life time of
                        plugin will be longer than methods it returns.
          \return List of methods presented to user
         */
        virtual QList <MethodBase *> methods (QObject * parent) = 0;
        
        
    Q_SIGNALS:
    
        /*!
          \brief Signal to be emitted if new method becomes available after
                 <code>methods</code> function has returned the original
                 list of methods. If signal is emitted before
                 <code>methods</code> function is called then it's ignored.
          \param method New method provided by plugin
         */
        void newMethod (ShareUI::MethodBase * method);
    };
}

#endif // #ifndef _SHARE_UI_PLUGIN_BASE_H_  
