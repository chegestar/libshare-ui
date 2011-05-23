 
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

#ifndef _SHARE_UI_ITEM_CONTAINER_PRIVATE_H_
#define _SHARE_UI_ITEM_CONTAINER_PRIVATE_H_

#include <QObject>
#include <QList>
#include <ShareUI/Item>
#include <ShareUI/ItemContainer>
#include <QSharedPointer>
#include <QStringList>

namespace ShareUI {

    /*!
        \class ItemContainerPrivate
        \brief Private class for ItemContainer
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class ItemContainerPrivate : public QObject {
    
    Q_OBJECT
    
    public:
    
        /*!
          \brief Constructor
          \param sizeLimit Size limit of container. 0 == no size limit.
         */
        ItemContainerPrivate (ItemContainer * parent = 0, int sizeLimit = 0);
        
        virtual ~ItemContainerPrivate();
        
        /*!
          \brief Append items to container
          \param constructInfo information
          \return <code>true</code> if items successfully added
         */
        bool appendItems (const QStringList & constructInfo);
        
        /*!
          \brief Clean entries from list that are found in find list
          \param list List of constructinfo
          \param find List of Item which constructinfo is compared to list
         */
        void cleanList (QStringList & list, 
            QList<QSharedPointer<Item> > & find);
    
        int m_sizeLimit; //!< Size limit for the container
        
        //! Items in container
        QList<QSharedPointer<Item> > m_items;
        
        //! Items not yet ready
        QList<Item*> m_waitingItems;
        
    public Q_SLOTS:
    
        /*!
          \brief Slot for items ready signals
         */
        void itemReady ();
        
    Q_SIGNALS:
        
        /*!
          \brief Signal to be connected to parents identical signal
         */
        void itemsReady();   
     
    };
}

#endif
