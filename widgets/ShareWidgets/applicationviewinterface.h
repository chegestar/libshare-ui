
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

#ifndef _SHARE_WIDGETS_APPLICATION_VIEW_INTERFACE_H_
#define _SHARE_WIDGETS_APPLICATION_VIEW_INTERFACE_H_

#include <ShareWidgets/export.h>
#include <ShareUI/PluginLoader>
#include <ShareUI/ItemContainer>
#include <QObject>
#include <ShareWidgets/PresentationWidgetInterface>

namespace ShareWidgets {

    /*!
      \brief Interface definition for functionality all Share UI pages should
             offer.
     */
    class SHARE_WIDGETS_EXPORT ApplicationViewInterface {
    public:
    
        virtual ~ApplicationViewInterface() {}
            
        /*!
          \brief Item container given to page
          \return Container given to and used by page
         */        
        virtual const ShareUI::ItemContainer * itemContainer() const = 0;
        
        /*!
          \brief Set item container used in page
         */
        virtual void setItemContainer (
            const ShareUI::ItemContainer * container) = 0; 
        
        /*!
          \brief Tell what pluginLoader to use
          \param loader Loader to be used
         */
        virtual void setPluginLoader (ShareUI::PluginLoader * loader) = 0;
        
        virtual ShareUI::PluginLoader * pluginLoader() const = 0;
        
        /*!
          \brief Returns current presentation widget if set
          \return Current presentation widget
         */        
        virtual PresentationWidgetInterface * presentationWidget () const = 0;
        
        /*!
          \brief Function to set presentation widget for view
          \param widget Widget used as presentation widget
         */
        virtual void setPresentationWidget (
            PresentationWidgetInterface * widget) = 0;        
        
        /*!
          \brief Returns current main widget if set. Main widget is part of UI
                 where user can do changes to share options and targets.
          \return Current main widget
         */
        virtual QObject * mainWidget () const = 0;
        
        /*!
          \brief Visibility of share button.
          \return true if button is shown to user
         */
        virtual bool shareButtonVisible () const = 0;
                
        /*!
          \brief Way to check if share button is enabled in UI.
          \return In default implementation always false
         */
        virtual bool shareButtonEnabled () const = 0;
        
        /*!
          \brief Sets new main widget to application page. Will delete old
                 main widget if defined earlier. It's up to UI implementation
                 to cast class to correct QObject based class.
          \param widget New widget
         */
        virtual void setMainWidget (QObject * widget) = 0;        
    
        /*!
          \brief Way to set share button visible in UI or hide it.
          \param show If button should be shown or not
         */
        virtual void setShareButtonVisible (bool show) = 0;    
    
        /*!
          \brief Way to set share button enabled in UI or disable it.
          \param show If button is enabled or not
         */    
        virtual void setShareButtonEnabled (bool enabled) = 0;    

    protected:
    
        /*!
          \brief Signal to be emitted from implementing class when user action
                 has been done and Share UI application should destroy window
                 and shutdown.
         */
        virtual void shutdown() = 0;
    
        /*!
          \brief Signal emitted when share button is clicked. Actual emiting
                 must be done by inheriting classes.
         */
        virtual void shareButtonClicked () = 0;
    };
}

#endif
