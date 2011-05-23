 
/*
 * share-ui -- Handset UX Share user interface
 * Copyright (c) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
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
 
#ifndef _SHARE_UI_SHARED_METHOD_DATA_H_
#define _SHARE_UI_SHARED_METHOD_DATA_H_

#include <ShareWidgets/export.h>
#include <QObject>
#include <QString>
#include <ShareWidgets/ApplicationViewInterface>

namespace ShareWidgets {

    class SharedMethodDataPrivate;

    /*!
      \class SharedMethodData
      \brief Share data for the methods
     */
    class SHARE_WIDGETS_EXPORT SharedMethodData : public QObject {
    Q_OBJECT
    public:
       
        /*!
          \brief Get instance
          \return Pointer to shared data
         */
        static SharedMethodData * instance();
    
        virtual ~SharedMethodData ();
        
        /*!
          \brief Application page where method is presented
          \param caller For which method page is requested
          \return Pointer to application page or null if not defined
         */
        ShareWidgets::ApplicationViewInterface * applicationView (
            ShareUI::MethodBase * caller) const;
        
        /*!
          \brief For systems loading methods to register method's data
          \param method Method registered
          \param page Page on which method is presented
         */
        void registerMethod (ShareUI::MethodBase * method,
            ShareWidgets::ApplicationViewInterface * iface);
        
        /*!
          \brief Unregister all information stored for method
          \param method Method unregisted
         */
        void unregisterMethod (ShareUI::MethodBase * method);

    private:
    
        Q_DISABLE_COPY (SharedMethodData)
    
        /*!
          \brief Private constructor. Use instance to get pointer.
          \param parent QObject parent
         */
        SharedMethodData (QObject * parent = 0);    
    
        static SharedMethodData * m_instance; //!< current instance
        SharedMethodDataPrivate * const d_ptr; //!< private data
        
    };
}

#endif
