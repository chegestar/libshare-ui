
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
          \brief Interface function to ask for new application view from the
                 Share UI implementation
          \param pluginLoader Plugin loader given to page (with plugins already
                              loaded)
          \param container Content shared and presented on page
          \param parent Parent for MApplicationPage
          \return Pointer to view constructed by plugin
         */
        virtual ApplicationViewInterface * newApplicationView (
            ShareUI::PluginLoader * pluginLoader,
            ShareUI::ItemContainer * container) = 0;
        
    };
}

Q_DECLARE_INTERFACE(ShareWidgets::UiImplementation,
    "com.meego.share-widgets.ui-implementation/0.1");

#endif