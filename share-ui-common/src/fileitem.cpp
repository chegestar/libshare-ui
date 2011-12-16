 
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

#include "ShareUI/FileItem"
#include "fileitem_p.h"
#include <QtSparql>
#include <QFileInfo>
#include <QDebug>
#include <contentinfo.h>

using namespace ShareUI;

#define STREAM_PREFIX "FileItem:"
#define DBG_STREAM qDebug() << STREAM_PREFIX
#define WARN_STREAM qWarning() << STREAM_PREFIX
#define CRIT_STREAM qCritical() << STREAM_PREFIX


FileItem::FileItem (const QString & constructInfo, QObject * parent) :
    Item (constructInfo, parent), d_ptr (new FileItemPrivate()) {

    connect (d_ptr, SIGNAL(ready()), this, SIGNAL(ready()));
    connect (d_ptr, SIGNAL (thumbnail(QPixmap)), this,
        SIGNAL (thumbnail(QPixmap)));
}

FileItem::~FileItem () {
    delete d_ptr;
}

SharedItem FileItem::create (const QString & constructInfo) {

    QStringList constructInfoList;
    constructInfoList << constructInfo;

    QList<SharedItem> list = createList (constructInfoList);
    SharedItem item;
    // Ideally, list should never have size more than 1
    if (list.size () >= 1) {
        item = list [0];
    }

    return item;
}

QList<QSharedPointer<Item> > FileItem::createList (
    const QStringList & constructInfoList) {

    QList<QSharedPointer<Item> > list;

    static QSparqlConnection connection("QTRACKER");
    if (!connection.isValid()) {
        CRIT_STREAM << "Tracker driver for QtSparql not found";
        return list;
    }

    DBG_STREAM << __FUNCTION__ << "start";

    QStringList fileUrlList;
    QString fileUrls;
    QList<QUrl> trackerUriList;
    QString trackerUris;

    int itemCount = constructInfoList.count();
    
    // For each item in constructInfoList, check if it is a tracker IRI or a
    // fileURI
    for (int i = 0; i < itemCount; ++i) {
        
        bool isTrackerIri = true;
    
        QUrl fUrl (constructInfoList [i]);
        
        if (constructInfoList [i].startsWith ("/") == true) {
            // Input wasn't url, use fromLocalFile to fix this
            fUrl = QUrl::fromLocalFile (constructInfoList [i]);
            isTrackerIri = false;
        
        } else if (constructInfoList [i].startsWith ("file://") == true) {
            fUrl = FileItemPrivate::unencodedUrl(fUrl);
            isTrackerIri = false;
            
        } else if (constructInfoList [i].startsWith ("urn:") == false) {
            qDebug() << constructInfoList[i]
                << "is not tracker file address or file path/uri";
            continue;
        }
    
        if (isTrackerIri == true) {
            trackerUriList.append (fUrl);
            trackerUris.append (QString("'%1',").arg(fUrl.toString()));
        } else {
            //Make sure URI has proper encoding
            // QUrl::toEncoded does not work since it does not encode the ';'
            // character (bug 226163). Instead use toPercentEncoding to
            // converting everything possible, except for the characters given
            // in the except list to the corresponding percent encoding
            QByteArray encodedUrl = 
                QUrl::toPercentEncoding (fUrl.toString (), ":/()=&@'!,$+");
            if (itemCount > 1 && encodedUrl.indexOf ('\'') >= 0) {
                encodedUrl.insert (encodedUrl.indexOf ('\''), "\\");
                qDebug() << "Escaped the \' character";
            }
            fileUrlList.append (encodedUrl);
            fileUrls.append (QString("'%1',").arg(QString(encodedUrl)));
        }

        // Try to predict required memory usage and avoid repeated reallocations.
        // Assume that all the items will be of the same type (file URL or
        // tracker URI).
        if (i == 0) {
            if (!trackerUriList.isEmpty()) {
                trackerUriList.reserve(itemCount);
                trackerUris.reserve(trackerUris.size() * itemCount);
            }
            else if (!fileUrlList.isEmpty()) {
                fileUrlList.reserve(itemCount);
                fileUrls.reserve(fileUrls.size() * itemCount);
            }
        }
    }
    
    // Nothing to query
    if (trackerUriList.isEmpty() == true && fileUrlList.isEmpty() == true) {
        return list;
    }

    // Remove the last ',' from the strings
    trackerUris.chop(1);
    fileUrls.chop(1);

    QString queryStr = QString(
        "SELECT ?f tracker:id(?f) ?url ?mime ?size ?title ?desc ?created "
        "fn:timezone-from-dateTime(?created) ?modified ?duration "
        "WHERE {?f nie:url ?url ; nie:mimeType ?mime . "
        "OPTIONAL { ?f nie:byteSize ?size } "
        "OPTIONAL { ?f nie:title ?title } "
        "OPTIONAL { ?f nie:description ?desc } "
        "OPTIONAL { ?f nie:contentCreated ?created } "
        "OPTIONAL { ?f nfo:fileLastModified ?modified } "
        "OPTIONAL { ?f nfo:duration ?duration } ");

    QSparqlQuery query;

    if (itemCount == 1) {
        queryStr += '}';
        if (!trackerUris.isEmpty()) {
            queryStr.replace("?f", "?:f");
            query.setQuery(queryStr);
            query.bindValue("f", QUrl(trackerUris.mid(1, trackerUris.length() - 2)));
        }
        else {
            queryStr.replace("?url", "?:url");
            query.setQuery(queryStr);
            query.bindValue("url", fileUrls.mid(1, fileUrls.length() - 2));
        }
    }
    else {
        queryStr += QString("FILTER (str(?f) in (%1) || ?url in (%2)) }")
            .arg(trackerUris)
            .arg(fileUrls);
        query.setQuery(queryStr);
    }

    DBG_STREAM << "Making Sparql query";

    QSparqlResult *result = connection.exec(query);
    result->waitForFinished();
    int rowCount = 0;
    if (result->hasError()) {
        WARN_STREAM << "Sparql query failed:" << result->lastError().message();
    }
    else {
        DBG_STREAM << "Sparql query ok";
        rowCount = result->size();
    }

    QStringList input (constructInfoList);
    DBG_STREAM << "Tracker reply has" << rowCount << "row(s)";

    list.reserve(rowCount);
    
    for (int row = 0; row < rowCount; ++row) {
        result->next();
        QString tIri = result->binding(0).value().toString ();
        int trackerId = result->binding(1).value().toInt ();
        QString fUri = result->binding(2).value().toString ();
        QString mime = result->binding(3).value().toString ();
        
        if (fUri.isEmpty() || mime.isEmpty()) {
            DBG_STREAM << "Ignoring tracker item" << tIri << fUri << mime;
            continue;
        }     

        QString fileSizeStr = result->binding(4).value().toString ();
        quint64 fileSize = 0;
        if (fileSizeStr.isEmpty () == false) {
            fileSize = fileSizeStr.toInt ();
        }
    
        QString fileTitle = result->binding(5).value().toString ();
        QString fileDesc = result->binding(6).value().toString ();
        QDateTime created = result->binding(7).value().toDateTime();
        int localTimeZoneInSeconds =
            qRound(result->binding(8).value().toString().toDouble());
        created = created.addSecs(localTimeZoneInSeconds);
        created.setTimeSpec(Qt::LocalTime);
        QDateTime modified = result->binding(9).value().toDateTime();
        int duration = result->binding(10).value().toInt();

        QUrl unencodedFilePathUri = FileItemPrivate::unencodedUrl (fUri);

        FileItem * fileItem = new FileItem (tIri);
        fileItem->d_ptr->m_trackerIri  = QUrl (tIri);
        fileItem->d_ptr->m_trackerId = trackerId;
        fileItem->d_ptr->m_filepathUri = unencodedFilePathUri;
        fileItem->d_ptr->m_mime = mime;
        fileItem->d_ptr->m_bytes = fileSize;
        fileItem->d_ptr->m_title = fileTitle;
        fileItem->d_ptr->m_desc = fileDesc;
        fileItem->d_ptr->m_duration = duration;
        fileItem->d_ptr->m_contentCreated = created;
        fileItem->d_ptr->m_lastModified = modified;
        fileItem->d_ptr->m_encodedFileUrl = QUrl::fromEncoded(fUri.toUtf8());
        fileItem->d_ptr->m_sparqlConnection = &connection;

        QSharedPointer<Item> item (fileItem);
        list.append (item);
    }

    delete result;
    result = 0;
    
    DBG_STREAM << (itemCount - list.size()) << "item(s) not accepted";
    DBG_STREAM << __FUNCTION__ << "end";

    return list;
}

QString FileItem::mimeType () {
    return d_ptr->m_mime;
}

QUrl FileItem::trackerIri () const {
    return d_ptr->m_trackerIri;
}

QString FileItem::fileTitle () const {
    return d_ptr->m_title;
}

QString FileItem::fileDescription () const {
    return d_ptr->m_desc;
}

QString FileItem::fileUri () const {
    return d_ptr->m_filepathUri.toEncoded();
}

QUrl FileItem::URI() const {
   return d_ptr->m_encodedFileUrl;
}

QString FileItem::filePath () const {
   return d_ptr->m_encodedFileUrl.toLocalFile ();
}

int FileItem::duration () const {
    return d_ptr->m_duration;
}

QDateTime FileItem::contentCreated () const {
    return d_ptr->m_contentCreated;
}

QDateTime FileItem::lastModified () const {
    return d_ptr->m_lastModified;
}

quint64 FileItem::size() const {
    return d_ptr->m_bytes;
}

QString FileItem::title() {
    QFileInfo fInfo (filePath ());
    return fInfo.fileName ();
}

QString FileItem::description() {
    //TODO: Should we return something?
    return QString();
}

QString FileItem::icon() {
    return d_ptr->mimeIconName ();
}

bool FileItem::isReady () {
    
    bool retVal = false;
    if ( d_ptr->m_bytes > 0.0 ) {
        retVal =  true;
    } else if (d_ptr->m_waitingForSize == true ) {
        retVal =  false;
    } else {
        d_ptr->m_waitingForSize = true;
        d_ptr->updateFromTracker();
        if (d_ptr->m_bytes <= 0.0 ) {
            QString className;
            if (d_ptr->m_mime.contains("image", Qt::CaseInsensitive)) {
                className = "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image";
            }
            else if (d_ptr->m_mime.contains("video", Qt::CaseInsensitive)) {
                className = "http://www.tracker-project.org/temp/nmm#Video";
            }
            else if (d_ptr->m_mime.contains("audio", Qt::CaseInsensitive)) {
                className = "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Audio";
            }
            else {
                WARN_STREAM << "Unable to create change notification for mime type" << d_ptr->m_mime;
            }
            if (!className.isEmpty()) {
                d_ptr->m_file_signaler = new TrackerChangeNotifier(className);
            }
            if(d_ptr->m_file_signaler != 0)
            {
                DBG_STREAM << "Size not available yet, change notifier created";
                connect(d_ptr->m_file_signaler, 
                    SIGNAL(changed(QList<TrackerChangeNotifier::Quad>,
                                   QList<TrackerChangeNotifier::Quad>)),
                    d_ptr,
                    SLOT(fileChanged(QList<TrackerChangeNotifier::Quad>,
                                 QList<TrackerChangeNotifier::Quad>)));
            }
            retVal =  false;
        } else {
            d_ptr->m_waitingForSize = false;
            retVal =  true;
        }
    }
    return retVal;
}

// -- private functions -------------------------------------------------------
FileItemPrivate::FileItemPrivate (FileItem * parent) : QObject (parent),
    m_bytes (0), m_duration(0), m_trackerId(0),
    m_file_signaler (0), m_parent (parent) {
    
    m_waitingForSize = false;
}

FileItemPrivate::~FileItemPrivate () {
    if (m_file_signaler != 0 ) {
        delete m_file_signaler;
        m_file_signaler = 0;
    }
}

QUrl FileItemPrivate::unencodedUrl (QUrl url) {
    QByteArray array = url.toString().toUtf8();

    return QUrl::fromEncoded (array);
}

void FileItemPrivate::updateFromTracker() {

    DBG_STREAM << "Updating item data from tracker";

    QSparqlQuery query(
        "SELECT ?size ?created fn:timezone-from-dateTime(?created) ?modified ?duration { "
        "OPTIONAL { ?:u nie:byteSize ?size } "
        "OPTIONAL { ?:u nie:contentCreated ?created } "
        "OPTIONAL { ?:u nfo:fileLastModified ?modified } "
        "OPTIONAL { ?:u nfo:duration ?duration } }");
    query.bindValue("u", m_trackerIri);
    QSparqlResult *result = m_sparqlConnection->exec(query);
    result->waitForFinished();
    if (!result->hasError()) {
        if (result->next()) {
            m_bytes = result->binding(0).value().toULongLong();
            m_contentCreated = result->binding(1).value().toDateTime();
            int localTimeZoneInSeconds = result->binding(2).value().toInt();
            m_contentCreated = m_contentCreated.addSecs(localTimeZoneInSeconds);
            m_contentCreated.setTimeSpec(Qt::LocalTime);
            m_lastModified = result->binding(3).value().toDateTime();
            m_duration = result->binding(4).value().toInt();
        }
    }
    else {
        DBG_STREAM << "Sparql query for item data failed:" <<
            result->lastError().message();
    }
}

void FileItemPrivate::fileChanged(QList<TrackerChangeNotifier::Quad> deletes,
                                  QList<TrackerChangeNotifier::Quad> inserts) {
    Q_UNUSED(deletes);

    DBG_STREAM << "fileChanged signal from tracker";

    bool match = false;
    Q_FOREACH(const TrackerChangeNotifier::Quad &q, inserts) {
        if (q.subject == m_trackerId) {
            match = true;
            break;
        }
    }

    if (match) {
        updateFromTracker();
        if (m_bytes > 0.0 ) {
            m_file_signaler->disconnect();
            m_file_signaler->deleteLater();
            m_file_signaler = 0;
            m_waitingForSize = false;
            Q_EMIT(ready());
        }
    }
}

QString FileItemPrivate::mimeIconName () {
    ContentInfo cinfo = ContentInfo::forMime (m_mime);

    if (cinfo.isValid () == false) {
        return QLatin1String ("icon-m-content-file-unknown");
    } else {
        return cinfo.typeIcon ();
    }
}

