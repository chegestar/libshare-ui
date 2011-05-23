 
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

#ifndef _SHARE_UI_ITEM_CONTAINER_H_
#define _SHARE_UI_ITEM_CONTAINER_H_

#include <ShareUI/export.h>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QListIterator>
#include <QSharedPointer>
#include <ShareUI/Item>

namespace ShareUI {

    class ItemContainerPrivate;
    
    typedef QListIterator<SharedItem > ItemIterator;
    
    /*!
        \class ItemContainer
        \brief Class containing items currently selected in UI. Providing
               signals when changes to this group is done.
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class SHARE_UI_EXPORT ItemContainer : public QObject {

    Q_OBJECT
    
    public:
    
        /*!
          \brief ItemContainer constructor
          \param sizeLimit Maxium amount of items in container. If 0 then
                 size is not limited.
          \param parent See QObject
         */
        ItemContainer (int sizeLimit = 0, QObject * parent = 0);

        virtual ~ItemContainer ();
        
        /*!
          \brief Generate an item from the given construction information and
                 then append the created item to the container
          \param constructInfo Construct information for item
         */
        void appendItem (const QString & constructInfo);
        
        /*!
          \brief If container is empty
          \return <code>true</code> if container is empty
         */
        bool isEmpty() const;
                
        /*!
          \brief Number or items in container
          \return Number of items in container
         */
        int count() const;
        
        /*!
          \brief Get item at given index
          \param index: Index of required item
          \return Item at given index
         */
        SharedItem getItem (int index) const;

        /*!
          \brief Get iterator for items in the container
          \return Iterator
         */
        ItemIterator itemIterator() const;
        
        /*!
          \brief Function to check if there is still free space for new items in
                 the container
          \return <code>true</code> if container is not full
         */
        bool hasSpace() const;
        
        /*!
          \brief To check if all items are ready to be shared. Signal itemsReady
                 is emitted only after this function is called and it has
                 returned <code>false</code>.
          \return <code>true</code> if items are ready
         */
        bool isReady() const;
        
        /*!
          \brief Get total size of all items in container
          \return Total size of items in bytes when all items are ready. If
                  asked before that will always return 0.
         */
        quint64 totalSize() const;
        
        /*!
          \brief Get count of items not fully processed yet
          \return Count of items not fully processed
         */
        int waitingItemsCount () const;
        
    public Q_SLOTS:
    
        /*!
          \brief Slot to generate items from given construction information and
                 then to append items created to container. If construction
                 information isn't accepted items are not made for those inputs.
          \param constructInfo Construct information for items
         */
        void appendItems (const QStringList & constructInfo);
        
    Q_SIGNALS:
    
        /*!
          \brief Signal emitted when container items are changed. This happens
                 if items are added or removed from the container.
         */
        void changed ();
        
        /*!
          \brief Signal emitted when all items are ready
         */
        void itemsReady();
            
    private:
    
        ItemContainerPrivate * const d_ptr; //!< Private class
            

    };
}

#endif
