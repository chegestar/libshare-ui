
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

#ifndef _SHARE_UI_IMPLEMENTATION_H_
#define _SHARE_UI_IMPLEMENTATION_H_

#include <ShareUI/PluginLoader>
#include <ShareUI/ItemContainer>
#include <QGraphicsItem>

namespace ShareWidgets {

    /*!
      \class UiImplementation
      \brief Interface class for ShareUI user interface implementations. You
             only need to inherit from this class if you want to rewrite whole
             ShareUI implementation.
     */
    class UiImplementation {
    public:
    
        virtual ~UiImplementation()  {}
        
        /*!
          \brief Asks for application class needed for implementation (if used
                 Qt based framework needs something that isn't QApplication)
          \param argc Reference to the argc passed to the main function -
                 integer value containing the number of arguments passed to the
                 application. 
                 PLEASE NOTE: Always use reference - some implementations of
                 QCoreApplication store reference to the argc, and if pass by
                 value is done, then that can result in a crash when argc is
                 accessed later, since the reference will be to some location
                 on stack whose value has changed since
          \param argv argument values
          \return Pointer to application created by plugin (it will be executed
                  after this function has returned)
         */
        virtual QApplication * application (int & argc, char ** argv) = 0;
    
        /*!
          \brief The implementing class will implement presenting the share UI
                 to the user when this function is called. The implementing
                 class should set the ownership of the arguments appropriately,
                 to ensure that they are deleted when no longer needed
          \param container List of items to be shared
          \param loader Loader to be used
         */
        virtual bool showUI (ShareUI::PluginLoader * loader,
            ShareUI::ItemContainer * container) = 0;
    };
}

Q_DECLARE_INTERFACE(ShareWidgets::UiImplementation,
    "com.meego.share-widgets.ui-implementation/0.1");

#endif
