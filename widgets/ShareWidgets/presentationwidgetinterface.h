
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

#ifndef _SHARE_WIDGETS_PRESENTATION_WIDGET_INTERFACE_H_
#define _SHARE_WIDGETS_PRESENTATION_WIDGET_INTERFACE_H_

#include <ShareWidgets/export.h>
#include <ShareUI/ItemContainer>
#include <ShareUI/Item>

namespace ShareWidgets {

    /*!
      \class PresentationWidgetInterface
      \brief Presentation widget interface
     */
    class SHARE_WIDGETS_EXPORT PresentationWidgetInterface {
       
    public:
    
        virtual ~PresentationWidgetInterface() {}    
               
        /*!
          \brief Item container used in presentation widget
          \return Pointer to container or null
         */
        virtual const ShareUI::ItemContainer * itemContainer () const = 0;
        
        /*!
          \brief Set item container widget should use
          \param Pointer to item container to be used
         */
        virtual void setItemContainer (
            const ShareUI::ItemContainer * container) = 0;
        
    protected:
    
        /*!
          \brief Signal emitted by presentation widget when item is selected. Up
                 to implementation if presentation widget supports selection.
          \param item Item selected by user
         */
        virtual void itemSelected (const ShareUI::Item * item) = 0;
        
    };
}

#endif
