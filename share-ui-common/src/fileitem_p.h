 
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

#ifndef _SHARE_UI_FILE_ITEM_PRIVATE_H_
#define _SHARE_UI_FILE_ITEM_PRIVATE_H_

#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QObject>
#include <QPixmap>
#include <TrackerChangeNotifier>

class QSparqlConnection;

namespace ShareUI {

    class FileItem;

    /*!
        \class SUIFilePrivate
        \brief Class containing private data of SUIFile
        \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class FileItemPrivate : public QObject {
    
        Q_OBJECT

    public:
        FileItemPrivate (FileItem * parent = 0);
        virtual ~FileItemPrivate ();
    
        /*!
          \brief Tracker gives encoded url. This causes trouble when
                 changed to filepath, since Qt file functions don't work.
                 This converts it into an unencoded url.
          \param url Url got from tracker
          \return Unencoded url
         */
        static QUrl unencodedUrl (QUrl url);
        
        /*!
          \brief Get icon name for item (file type icon)
          \return icon name
         */
        QString mimeIconName ();

        /*!
          \brief Checks whether the mime suffix is for a text file or not
          \param suffix Suffix of the mime. It is assumed that the prefix is
                    either application/ or text/ when this is called
          \return <code>true</code> if this is a text file else
                    <code>false</code>
         */
        static bool isTextMime (const QString & suffix);

        /*!
          \brief Checks whether the mime suffix is for a presentation or not
          \param suffix Suffix of the mime. It is assumed that the prefix is
                    either application/ or text/ when this is called
          \return <code>true</code> if this is a presentation else
                    <code>false</code>
         */
        static bool isPresentationMime (const QString & suffix);

        /*!
          \brief Checks whether the mime suffix is for a spreadsheet or not
          \param suffix Suffix of the mime. It is assumed that the prefix is
                    either application/ or text/ when this is called
          \return <code>true</code> if this is a spreadsheet else
                    <code>false</code>
         */
        static bool isSpreadSheetMime (const QString & suffix);
    
        QUrl m_filepathUri; //!< file path
        QUrl m_trackerIri; //!< tracker path
        QString m_mime; //!< mime type
        quint64 m_bytes; //!< size of file in bytes

        QString m_title; //!< title of the file
        QString m_desc; //!< description of the file
        int m_duration; //!< duration of the file in seconds
        QDateTime m_contentCreated; //!< when the file was created
        QDateTime m_lastModified; //!< when the file was last modified

        int m_trackerId;
        
        //< varible to check if waiting to get size is on in case of delayed
        //sharing
        bool m_waitingForSize; 
        
        TrackerChangeNotifier *m_file_signaler; //!< tracker change listener
        
        ShareUI::FileItem * m_parent;

        QSparqlConnection *m_sparqlConnection;
        
        void updateFromTracker();

    public Q_SLOTS:
    
        void fileChanged(QList<TrackerChangeNotifier::Quad> deletes,
                         QList<TrackerChangeNotifier::Quad> inserts);

    Q_SIGNALS:
        /*!
          \brief Signal emitted when item is ready to be shared. Only emitted if
          isReady is called and false returned.
         */
        void ready();
        
        /*!
          \brief Signal emitted when thumbnail has been received for item
         */
        void thumbnail (QPixmap outputThumbnail);
    };
}

#endif //#ifndef _SHARE_UI_FILE_PRIVATE_H_