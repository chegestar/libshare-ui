 
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
 
#ifndef _SHARE_UI_METHOD_BASE_H_
#define _SHARE_UI_METHOD_BASE_H_

#include <ShareUI/export.h>
#include <QObject>
#include <QString>

namespace ShareUI {

    class ItemContainer;

    /*!
      \class MethodBase
      \brief Base class for Share UI method classes created by plug-ins.
      \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
    */
    class SHARE_UI_EXPORT MethodBase : public QObject {

        Q_OBJECT
        Q_ENUMS(Type)
    
    public:
    
        /*!
          \brief Constructor
          \param parent QObject parent
         */
        MethodBase (QObject * parent = 0);
            
        virtual ~MethodBase ();
        
        //! Method types. Are used to order items in UI view.
        enum Type {
            TYPE_OTHER = 0, //!< Method does not belong to other groups
            TYPE_WEB_SERVICE = 1, //!< Method is related to web services 
            
            /*! Promoted, this type is accepted only for predefined plugins.
                If given from other plugins it will be converted to TYPE_OTHER.
             */
            TYPE_PROMOTED = 2,
            
            TYPE_N //!< Last enumeration value. Do not use.
        };
        
        /*!
          \brief Get type for method. Used to group methods in UI. Invalid
                 values will be converted to <code>TYPE_OTHER</code>.
          \return Type of method. Default implementation will return
                  <code>TYPE_OTHER</code>
         */
        virtual Type type ();
        
        /*!
          \brief Title for the method. For example "Facebook" or "Email".
                 Function has to be implemented by method and it must return
                 valid title to be shown with method.
          \return Method title.
         */
        virtual QString title () = 0;
        
        /*!
          \brief Subtitle for the method. For example "username".
                 Subtitles might be ignored in future UIs for some methods.
          \return Method subtitle. In default implementation empty string.
         */
        virtual QString subtitle ();
        
        /*!
          \brief Icon for the method
          \return Method icon. Can be name of icon or filepath to icon.
                  In default implementation empty string that is same as no
                  icon.
         */
        virtual QString icon ();
        
        /*!
          \brief ID for method. This is useful for applications using Method
                 plugins directly. For example id of email method is
                 "com.meego.email".
          \return ID string if defined by implementing class. By default empty
                  string.
         */      
        virtual QString id ();
        
        /*!
          \brief Ordering value for item. This is only used with TYPE_PROMOTED
                 methods. For other types it is not used. Just keep the default
                 implementation.
          \return Order value for method. Methods with higher order values are
                  before methods with lower order values. Default value is
                  0.
         */
        virtual int order ();

    public Q_SLOTS:
    
        /*!
          \brief Slot called at the beginning to make sure given content is
                 accepted by method. Implementation should emit visibility
                 signal to tell if method should be shown or not. This is always
                 called at least once before selected slot to avoid selection
                 if given content is not accepted by method.
          \param items Items currently open in Share UI
         */
        virtual void currentItems (const ShareUI::ItemContainer * items) = 0;
       
        /*!
          \brief Method selected. Handle sharing and emit back to return to
                 main UI or done if sharing has handled and Share UI should
                 quit.
          \param items Items to be shared
         */
        virtual void selected (const ShareUI::ItemContainer * items) = 0;
    
    Q_SIGNALS:
        
        /*!
          \brief Signal emitted when method has done it's part. And Share UI
                 should be closed as sharing has started. This signal will
                 tell Share UI application to shutdown.
         */
        void done ();
                
        /*!
          \brief Signal to tell ShareUI that selected handling failed and method
                 will not emit done or back signal.
          \param message Error message that can be presented to user.
          
         */
        void selectedFailed (QString message);
        
        /*!
          \brief Signal to tell if method should be shown or hidden. This signal
                 should be emitted after currentItems slot is called. Also if
                 there is other reasons to hide method (for example account
                 is disabled by user). Signal can be emitted also even with
                 old visibility value. It will be then ignored.
          \param show <code>true</code> if method should be shown to user.
         */
        void visible (bool show);

        /*!
          \brief Signal to request resending of items. Use this if you want
                 to check current visibility after method has been hidden for
                 another reason. For example account has been disabled and
                 so method has been hidden. When account gets enabled again you
                 still need to receive current item list to make sure if
                 method should be shown again.
         */
        void resendItems ();
        
    private:
   
         Q_DISABLE_COPY(MethodBase)
   
    };
}


#endif