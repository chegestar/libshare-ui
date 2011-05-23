
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

#ifndef _MEEGO_DATA_URI_H_
#define _MEEGO_DATA_URI_H_

#include <mdatauri-export.h>
#include <QObject>
#include <QUrl>
#include <QDebug>
#include <QString>

class MDataUriPrivate;

/*!
  \class MDataUri
  \brief Class for reading and creating data URIs
  \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
*/
class MDURI_EXPORT MDataUri {
public:

    /*!
      \brief constructor
    */
    MDataUri();

    /*!
      \brief constructor
    */
    MDataUri (const QUrl& uri);
    
    /*!
      \brief constructor
    */
    MDataUri (const QString& uriString);
    
    /*!
      \brief Copy constructor
    */
    MDataUri (const MDataUri & src);  
       
    virtual ~MDataUri();

    MDataUri &operator= (const MDataUri & src);
    MDataUri &operator= (const QUrl & src);    
    MDataUri &operator= (const QString & src);
    
    bool operator== (const MDataUri& other) const;
    bool operator== (const QUrl & other) const;
    bool operator== (const QString & other) const;

    /*!
      \brief Setup data URI from string presentation. If input is invalid it is
             ignored.
      \param stringUri String presentation of data URI
      \return <code>true</code> if the given data uri is valid. If invalid then
              <code>false</code>.
    */
    bool read (const QString& stringUri);

    /*!
      \brief Set the mime type for the URI
      \param mimeType New mime type for the URI
    */
    void setMimeType(const QString& mimeType);
    
    /*!
      \brief Mime type of URI
      \return Mime type of URI
    */
    QString mimeType() const;
    
    /*!
      \brief Check if the UR of mime type is of the given mime type.
      \param mimeType mime type to verify with the mime type of the URI
      \return <code>true</code> if the given mime type is equal to the mime
              type of the URI
    */
    bool isOfMimeType(const QString& mimeType) const;
    
    /*!
      \brief Set attribute value
      \param name Attribute name
      \param value Attribute value
    */
    void setAttribute(const QString& name, const QString& value);
    
    /*!
      \brief Get attribute value
      \param name Attribute name
      \return Value of attribute or empty string if not found
    */
    QString attribute (const QString& name) const;
        
    /*!
      \brief Check if the given attribute is present in the data URI
      \param name Attribute name
      \return <code>true</code> if attribute is defined for URI
    */
    bool hasAttribute(const QString& name) const;

    /*!
      \brief Get attribute iterator for attribute browsing
      \returns Iterator for attributes
    */
    QMapIterator<QString, QString> attributeIterator() const;
    
    /*!
      \brief Get the data of URI in binary format
    */
    QByteArray data() const;
       
    /*!
      \brief Get text data
      \return Text data or empty string if no text data
    */
    QString textData() const;

    /*!
        \brief Set text data to URI. For binary data use setBinaryData function.
        \param text data text
        \param charset Encoding to be used when URI is printed. Default is
                       ASCII. Also UTF-8 is supported.
    */
    void setTextData(const QString& text,
        const QString & charset = QLatin1String("us-ascii"));

    /*!
        \brief Set binary data to URI. Data will be base64 coded to output. For
               data than can be stored as test please use setTextData.
        \param data Binary data stored to URI
    */
    void setBinaryData(const QByteArray& data);
        
    /*!
      \brief String presentation of the data URI
    */
    QString toString() const;
    
    /*!
      \brief Check if the URI is valid or not. Valid data URI needs to have at
             least data part defined.
      \return <code>true</code> if the URI is valid
    */
    bool isValid() const;
    
    /*!
      \brief Generates file from the data of data URI
      \param filePath Path to file where data will be written. Will overwrite
                      if file already exists.
      \return <code>true</code> if data was written successfully
     */
    bool writeDataToFile (const QString & filePath) const;

private:    
    MDataUriPrivate * const d_ptr; //!< Private data
};

#endif
