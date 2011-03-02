
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
#include <ShareWidgets/ApplicationViewInterface>

class QApplication;

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
          \brief Get application pointer. The implementing class creates an
                 application pointer and returns it - it is upto the calling
                 function to handle destroying this pointer.
          \param argc Count of arguments passed
          \param argv List of arguments passed
          \return Application pointer
         */
        virtual QApplication * getApplicationPointer (int argc, char **argv) = 0;
    
        /*!
          \brief
          \param container List of items to be shared
          \param loader Loader to be used
         */
        virtual bool showUI (ShareUI::PluginLoader * loader,
            const ShareUI::ItemContainer * container) = 0;

    protected:

        /*! 
          \brief Signal to be emitted from implementing class when the 
                 UI has been made visible, and the method plugin loading 
                 can begin.
         */
        virtual void startLoadingPlugins() = 0;
    
        /*!
          \brief Signal to be emitted from implementing class when user action
                 has been done and Share UI application should destroy window
                 and shutdown.
         */
        virtual void shutdown() = 0;
    
    };
}

Q_DECLARE_INTERFACE(ShareWidgets::UiImplementation,
    "com.meego.share-widgets.ui-implementation/0.1");

#endif
