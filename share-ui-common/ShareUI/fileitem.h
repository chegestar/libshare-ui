
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

#ifndef _SHARE_UI_FILE_ITEM_H_
#define _SHARE_UI_FILE_ITEM_H_

#include <ShareUI/export.h>
#include <QObject>
#include <ShareUI/Item>
#include <QString>
#include <QUrl>
#include <QSharedPointer>
#include <QDateTime>

namespace ShareUI {

    class FileItemPrivate;
    
    /*!
        \class FileItem
        \brief File item class presenting file to be shared
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
    */
    class SHARE_UI_EXPORT FileItem : public Item {
        Q_OBJECT
    
    public:
        
        /*!
          \brief Generate File Item from Tracker IRI, file path or file URI.
          \param constructInfo A string that could be either the tracker IRI,
                    file path or file URI for a file
          \return Shared pointer to FileItem or null if input not accepted
         */
        static SharedItem create (const QString & constructInfo);
        
        /*!
          \brief Generate FileItem list from a list of string which provide
                 construction information for a file. Will query all items
                 once and so give performance boost.
          \param constructInfoList List of strings, each of which could be
                    either the tracker IRI, file path or file URI for a file
          \return List of shared pointers of FileItems of accepted input data
         */
        static QList<SharedItem > createList (
            const QStringList & constructInfoList); 
        
        /*!
          \brief Convert item to file item if possible
          \return Pointer to file item or null if given item isn't file item
         */
        static inline FileItem * toFileItem (SharedItem item) {
            return qobject_cast <FileItem* > (item.data());
        }
        
        virtual ~FileItem ();
        
        /*!
          \brief See ShareUI::Item::title()
          \return File title or name
         */
        virtual QString title();

        /*!
          \brief See ShareUI::Item::description()
          \return File size
         */
        virtual QString description();

        /*!
          \brief See ShareUI::Item::icon()
         */
        virtual QString icon();
                        
        /*!
          \brief Get mime type of file
          \return Mime type of file or empty string if undefined
         */
        virtual QString mimeType ();
           
        /*!
          \brief Get size type of file
          \return Size of file (only available after file is ready)
         */
        virtual quint64 size() const;
    
        /*!
          \brief Get path to file
          \return File path. Or empty string if undefined or file isn't
                  local.
         */
        QString filePath () const;
            
        /*!
          \brief Get file path uri for the file
          \return file path uri or empty
         */
        QString fileUri () const;
        
        /*!
          \brief Get file URI (percent encoded URI)
          \return URI or empty if not defined
         */
        QUrl URI() const;
            
        /*!
          \brief Get Tracker IRI for the file
          \return Tracker IRI
         */
        QUrl trackerIri () const;

        /*!
          \brief Returns the file title as in the tracker
          \return Title of the file
         */
        QString fileTitle () const;

        /*!
          \brief Returns the file description as in the tracker
          \return Description of the file
         */
        QString fileDescription () const;

        /*!
          \brief Returns the file duration
          \return Duration of the file in seconds, 0 if not applicable
         */
        int duration () const;

        /*!
          \brief Returns the date/time when the file was created
          \return Creation time/date, invalid QDateTime if not available
         */
        QDateTime contentCreated () const;

        /*!
          \brief Returns the date/time when the file was last modified
          \return Modification time/date, invalid QDateTime if not available
         */
        QDateTime lastModified () const;

        /*!
          \brief Implements Item::isReady. This to allow sharing of files not
                 yet written to filesystem.
          \return <code>true</code> if file is ready.
         */
        virtual bool isReady();
        
    protected:
    
        /*!
          \brief Protected constructor
         */
        FileItem (const QString & constructInfo, QObject * parent = 0);    
    
    private:
    
        Q_DISABLE_COPY(FileItem)
        FileItemPrivate * const d_ptr; //!< Private data

    };
}

#endif
