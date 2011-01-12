
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

#ifndef _MEEGO_DATA_URI_PRIVATE_H_
#define _MEEGO_DATA_URI_PRIVATE_H_

class MDataUriPrivate {

public:
    bool m_isBase64; //!< If content should be printed as binary data
    QString m_mimeType; //!< Mime type given for content
    QMap<QString, QString> m_attributes; //!< Attribute list
    QByteArray m_data; //!< Data stored to URI
    QString m_charset; //!< Charset given for content
    bool m_hasBinaryData; //!< If content is binary data

public:
    MDataUriPrivate();
    virtual ~MDataUriPrivate();
    
    void extractAttributes(QString& dataString);
    void extractData(QString& dataString);
    void extractCharset(QString& dataString);
    bool extract(const QString& uri);
    
    QString generateDataUri();
    
    /*!
      \brief Clear all data
     */
     void clear();
};

#endif