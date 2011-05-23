
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

#ifndef _SHARE_UI_ITEM_H_
#define _SHARE_UI_ITEM_H_

#include <ShareUI/export.h>
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QSize>
#include <QPixmap>

namespace ShareUI {

    /*!
      \class Item
      \brief Base class for items shared. See ShareUI::FileItem and
             ShareUI::DataURIItem.
      \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
    */
    class SHARE_UI_EXPORT Item : public QObject {

        Q_OBJECT
    
    public:

        virtual ~Item ();
        
        /*!
          \brief Title for item. Used in UI
          \return Title to be used with item
         */
        virtual QString title() = 0;
        
        /*!
          \brief Description of item. Used in UI
          \return Description to be used with item. Default implementation will
                  return empty string.
         */
        virtual QString description();
        
        /*!
          \brief Icon of item. Used in UI. This is called before loadThumbnail
                 slot. So it should return default icon for given type. If
                 item does not need thumbails then return value this gives
                 will be used.
          \return Icon name or path to it
         */
        virtual QString icon () = 0;
        
        /*!
          \brief Mime type of Item
          \return Mime type of item or empty if item doesn't have mime type.
                  Default implementation will return empty.
         */
        virtual QString mimeType();        
        
        /*!
          \brief String presentation of item. Used as default way to share item
                 forward. By default returns empty string.
          \return String presentation of item.
         */
        virtual QString toString();
        
        /*!
          \brief Item class should tell if item is ready to be shared or not. If
                 false is returned signal ready has to be emitted when item is
                 ready. User can't do selection until all items are ready. By
                 Default this will return always true. So implement this in your
                 class only if your items can be shared before those are ready.
          \return <code>true</code> if ready to be shared
         */
        virtual bool isReady();
        
        /*!
          \brief Get size of item in bytes
          \return Size of item in bytes. Default implementation will return 0.
         */
        virtual quint64 size() const;        
        
        /*!
          \brief Construction information for item
          \return String given to item when generated
         */
        QString constructInfo () const;        
        
   public Q_SLOTS:
        /*!
          \brief Slot called when item should start thumbnail loading. If item
                 does not need thumbnails then default implementation can be
                 used. It will not start thumbnail loading and so will not
                 emit thumbnail signal. thumbnail signals will be ignore if
                 those are emitted before this slot is called. So only start
                 thumbnail loading after this slot is called.
          \param size Size for thumbnail
         */
        virtual void loadThumbnail (QSize size);
            
    protected:
    
        /*!
          \brief Item constructor which is protected. Construction done via
                 implementing classes.
          \param constructInfo Contruction information used to generate item.
                               Can be used to identify items.
          \param parent QObject parent
         */
        Item (const QString & constructInfo, QObject * parent = 0);
        
        const QString m_constructInfo; //!< Construction information
        
    private:

         Q_DISABLE_COPY(Item)
         
    Q_SIGNALS:
        
        /*!
          \brief Signal emitted by implementing class when item is ready to be
                 shared. Only emitted if isReady is called and false returned.
         */
        void ready();
        
        /*!
          \brief Signal defining thumbnail that should be used with item
          \param pixmap Pixmap used with item
         */
        void thumbnail (QPixmap pixmap);
        
    };
    
    typedef QSharedPointer<Item> SharedItem;    
}

#endif
