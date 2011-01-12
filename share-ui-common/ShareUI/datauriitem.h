
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

#ifndef _SHARE_UI_DATA_URI_ITEM_H_
#define _SHARE_UI_DATA_URI_ITEM_H_

#include <ShareUI/export.h>
#include <QObject>
#include <ShareUI/Item>
#include <QString>
#include <QStringList>
#include <MDataUri>


namespace ShareUI {

    class DataUriItemPrivate;


    /*!
        \class DataUriItem
        \brief Class to provide Data URI object support to ShareUI FW
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
    */
    class SHARE_UI_EXPORT DataUriItem : public Item {
        Q_OBJECT
    
    
    public:

        /*!
          \brief Generate Data URI item from data uri
          \param dataUri Data URI containing the object information
          \return Shared pointer to Data URI item or null if URI was not accepted
         */
        static SharedItem create (const QString & dataUri);
        
        /*!
          \brief Create multiple DataUriItems from given Uris
          \param dataUris URI where DataUriItem should be made
          \return List of DataUriItems for accepted URIs
         */
        static QList<SharedItem > createList (const QStringList & dataUris);
        
        /*!
          \brief Convert item to data URI item if possible
          \return Pointer to data URI item or null if given item isn't file item
         */
        static inline DataUriItem * toDataUriItem (SharedItem item) {
            return qobject_cast <DataUriItem* > (item.data());
        }        

        virtual ~DataUriItem ();
        
        //! \reimp
        virtual QString title();        
        virtual QString description();
        virtual QString icon ();
        virtual QString toString();
        virtual QString mimeType ();
        //! \reimp_end
        
        /*!
          \brief Item's DataURI
          \return Reference to data URI inside item
         */
        virtual const MDataUri & dataUri() const;
        
    public Q_SLOTS:
    
        /*!
          \brief Implementation for Item::loadThumbnail. Will extract thumbnail
                 from DataURI
         */
        virtual void loadThumbnail (QSize size);
    
    protected:
    
        /*!
          \brief Protected constructor
          \param constructInfo Contruction information used to generate item.
                               Can be used to identify items.
          \param parent QObject parent
         */
        DataUriItem (const QString & constructInfo, QObject * parent = 0);
    
    private:
    
        Q_DISABLE_COPY(DataUriItem)
        DataUriItemPrivate * const d_ptr; //!< Private data

    }; 
}

#endif