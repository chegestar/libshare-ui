
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

//Qt Headers
#include <QUrl>
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QDir>
//local headers
#include "mdatauri.h"
#include "mdatauri_p.h"

namespace {
    const QLatin1String DEFAULT_MIME_TYPE("text/plain");
    const QLatin1String DATA_URI_SCHEME("data");
    const QLatin1String UTF8_ENCODING("utf-8");
    const QLatin1String US_ASCII_ENCODING("us-ascii");
    const QLatin1String CHARSET_STRING("charset");
    const QLatin1String BASE64_STRING("base64");
    const QChar FIELD_SEPARATOR(';');
    const QChar DATA_SEPARATOR(',');
    const QChar ATTRIBUTE_CHAR('=');
    const QChar MIME_TYPE_CHAR('/');
}

MDataUri::MDataUri() : d_ptr(new MDataUriPrivate()) {
}

MDataUri::MDataUri(const QString& uriString) : d_ptr(new MDataUriPrivate()) {
    read (uriString);
}

MDataUri::MDataUri(const QUrl& uri) : d_ptr(new MDataUriPrivate()) {
    read(uri.toString());
}

MDataUri::MDataUri (const MDataUri & src) : d_ptr(new MDataUriPrivate()) {
    read (src.toString());
}

MDataUri::~MDataUri() {
    delete d_ptr;
}

bool MDataUri::read(const QString& stringUri) {
    QUrl uri(stringUri);
    bool ret = false;
    
    if(uri.scheme() == DATA_URI_SCHEME && uri.host().isEmpty()) {
        ret = d_ptr->extract(stringUri);
    }
    
    return ret;
}

void MDataUri::setMimeType(const QString& mimeType) {
    d_ptr->m_mimeType = mimeType;
}

//mimetype
QString MDataUri::mimeType() const {
    QString retString;
    if(isValid()) {
        if(d_ptr->m_mimeType.isEmpty() == true) {
            retString = DEFAULT_MIME_TYPE;
        } else {
            retString = d_ptr->m_mimeType;
        }
    }
    return retString;
}

bool MDataUri::isOfMimeType(const QString& mimeType) const {
    return (mimeType == this->mimeType());
}


//Attributes
void MDataUri::setAttribute(const QString& name, const QString& value) {
    d_ptr->m_attributes.insert(name,value);
}

QString MDataUri::attribute(const QString& name) const {
    return d_ptr->m_attributes.value(name);
}

bool MDataUri::hasAttribute(const QString& name) const {
    return d_ptr->m_attributes.contains(name);
}

QMapIterator<QString, QString> MDataUri::attributeIterator() const {
    QMapIterator<QString, QString> iter(d_ptr->m_attributes);
    return iter;
}

//Data
QByteArray MDataUri::data() const {
    QByteArray dataArray;
    if(d_ptr->m_hasBinaryData == true) {
        dataArray = d_ptr->m_data;
    }
    return dataArray;
}

QString MDataUri::textData() const {
    QString ret;

    if(d_ptr->m_hasBinaryData == false) {       
        if (d_ptr->m_charset == UTF8_ENCODING) {
            ret = QString::fromUtf8 (d_ptr->m_data.constData(),
                d_ptr->m_data.size());
        } else {
            ret = QString::fromAscii (d_ptr->m_data.constData(),
                d_ptr->m_data.size());
        }
    }

    return ret;
}

void MDataUri::setTextData(const QString& text, const QString& charset) {
    bool inputOk = false;

    if (charset.isEmpty() == true ||
        charset.toLower() == US_ASCII_ENCODING) {
        
        d_ptr->m_charset.clear();
        d_ptr->m_data = text.toAscii();
        inputOk = true;
    } else if (charset.toLower() == UTF8_ENCODING) {
        d_ptr->m_charset = UTF8_ENCODING;
        d_ptr->m_data = text.toUtf8();
        inputOk = true;
    }

    if (inputOk == true) {
        d_ptr->m_hasBinaryData = false;
    }
}

void MDataUri::setBinaryData (const QByteArray& data) {
    d_ptr->m_data = data;
    
    d_ptr->m_hasBinaryData = true;
    d_ptr->m_isBase64 = true;
}

// convert URI to string
QString MDataUri::toString() const {

    if (isValid() == false) {
        return QString();
    }

    return d_ptr->generateDataUri();

}

bool MDataUri::isValid() const {
    return (d_ptr->m_data.isEmpty() == false);
}

// operators

MDataUri & MDataUri::operator = (const MDataUri & src) {
    if (this != &src) {
        this->d_ptr->clear();
        this->read (src.toString());
    }

    return *this;
}

MDataUri & MDataUri::operator = (const QUrl & src) {
    this->d_ptr->clear();
    this->read (src.toString());

    return *this;
}

MDataUri & MDataUri::operator = (const QString & src) {
    this->d_ptr->clear();
    this->read (src);

    return *this;
}

bool MDataUri::operator== (const QString& other) const {
    QString me = toString();
    return (me == MDataUri(other).toString());
}

bool MDataUri::operator== (const QUrl& other) const {
    QString b = other.toString();
    return (*this == b);
}

bool MDataUri::operator== (const MDataUri& other) const {
    QString b = other.toString();
    return (toString() == b);
}

bool MDataUri::writeDataToFile (const QString & filePath) const {
    bool success = true;
    QDir dir;
    
    if (QFile::exists (filePath) == true) {
        if (QFile::remove (filePath) == false) {
            qWarning() << "Can't remove file:" << filePath;
            success = false;
        }
    }
    
    if (success == true) {
        QFile file (filePath);
        
        if (file.open(QIODevice::WriteOnly) == false) {
            qWarning() << "Can't write to file:" << filePath;
            success = false;
        } else {
            qint64 writeRet = file.write (d_ptr->m_data);
            
            success = (writeRet != -1);
         
            file.close();
        }
    }
    
    return success;
}

//******************************* Private ************************************//
MDataUriPrivate::MDataUriPrivate() :  m_isBase64(false),
    m_hasBinaryData(false) {
}

MDataUriPrivate::~MDataUriPrivate() {
}

bool MDataUriPrivate::extract(const QString& uri) {

    QString stringUri = uri;

    // Remove "data:"
    stringUri.remove(0, QString(DATA_URI_SCHEME).length() + 1);

    // Split to fields.
    QStringList fields = stringUri.split(
        QRegExp(QString(FIELD_SEPARATOR) + '|' + DATA_SEPARATOR));

    // Must contain a data separator, if there is more than one field.
    if (fields.count() > 1 && !stringUri.contains(DATA_SEPARATOR))
        return false;

    clear();

    for (int i = 0; i < fields.count(); i++) {
        QString field = fields[i];
        if (i == fields.count() - 1) { // Last field = data
            if (m_isBase64 == true) {
                m_data = QByteArray::fromBase64(field.toAscii());
                m_hasBinaryData = true;
            }
            else {
                QString encString = QUrl::fromPercentEncoding(field.toAscii());
                if (m_charset.toLower() == UTF8_ENCODING) {
                    m_data = encString.toUtf8();
                } else {
                    m_data = encString.toAscii();
                }
            }
        }
        else if (i == 0 && field.contains(MIME_TYPE_CHAR) &&
            !field.contains(ATTRIBUTE_CHAR)) {

            m_mimeType = field;
        }
        else if (field.contains(ATTRIBUTE_CHAR)) {
            QStringList keyValue = field.split(ATTRIBUTE_CHAR);
            if (keyValue[0] == CHARSET_STRING) {
                m_charset = keyValue[1].toLower();
            }
            else {
                m_attributes.insert(
                    QUrl::fromPercentEncoding(keyValue[0].toAscii()),
                    QUrl::fromPercentEncoding(keyValue[1].toAscii()));
            }
        }
        else if (field == BASE64_STRING) {
            m_isBase64 = true;
        }
        else {
            // Field is discarded.
        }
    }

    if (m_mimeType.isEmpty() == true) {
        m_mimeType = DEFAULT_MIME_TYPE;
    }

    if (m_charset.isEmpty() == true && m_hasBinaryData == false) {
        m_charset = US_ASCII_ENCODING;
    }

    return (m_data.isEmpty() == false);
}

QString MDataUriPrivate::generateDataUri() {
    QString dataUri;
    dataUri.append(DATA_URI_SCHEME).append(':');
    if (m_mimeType.isEmpty() == true) {
        dataUri.append(DEFAULT_MIME_TYPE);
    } else {
        dataUri.append(m_mimeType);
    }
    
    if (m_attributes.count() > 0) {
        QMapIterator<QString, QString> iter(m_attributes);
        while (iter.hasNext()) {
             iter.next();
             dataUri.append(FIELD_SEPARATOR);
             dataUri.append(QUrl::toPercentEncoding(iter.key()));
             dataUri.append(ATTRIBUTE_CHAR);
             dataUri.append(QUrl::toPercentEncoding(iter.value()));
         }
    }

    if (m_charset.isEmpty() == false) {
        dataUri.append(FIELD_SEPARATOR);
        dataUri.append(CHARSET_STRING);
        dataUri.append(ATTRIBUTE_CHAR);
        dataUri.append(m_charset);
    }

    if (m_isBase64 == true) {
        dataUri.append(FIELD_SEPARATOR);
        dataUri.append(BASE64_STRING);
    } 

    dataUri.append(DATA_SEPARATOR);
    if (m_isBase64 == true) {
        dataUri.append(m_data.toBase64());
    } else {
        if (m_charset.isEmpty() || m_charset.toLower() == US_ASCII_ENCODING) {
            dataUri.append(QUrl::toPercentEncoding(m_data));
        } else if (m_charset.toLower() == UTF8_ENCODING) {
            QString string = QString::fromUtf8 (m_data.data(), m_data.size());
            dataUri.append(QUrl::toPercentEncoding(string));
        }
    }

    return dataUri;
}

void MDataUriPrivate::clear() {
    m_isBase64 = false;
    m_mimeType.clear();
    m_attributes.clear();
    m_data.clear();
    m_charset.clear();
    m_hasBinaryData = false;
}