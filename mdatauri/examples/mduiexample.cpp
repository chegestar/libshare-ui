
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

 /*!
 * \example mduiexample.cpp
 * Simple example how to contstruct and read datauris with MDataUri
 */
 
#ifndef _MDUI_EXAMPLE_H_
#define _MDUI_EXAMPLE_H_ 
 
#include <MDataUri>
#include <QString>
#include <QByteArray>

/*
 \class MDataUriExamples
 \brief Class containing example functions
*/
class MDataUriExamples {

public:

    // Example 1
    //-----------
    // Generate string format data uri from link
    //
    // Input param url: (eq. "http://www.meego.com")
    // Input param title: (eq. "MeeGo HomePage"), optional title that is stored
    //  as attribute to data URI
    // 
    // Return value is data uri containing given information (eq.
    //  "data:txt/x-url;title=MeeGo%20Homepage,http%3A%2F%2Fwww.meego.com"
    //    
    static QString linkUrlToDataUri (const QString & url,
        const QString & title = QString()) {
        
        MDataUri duri;
        
        duri.setMimeType ("text/x-url");
        duri.setTextData (url);
        
        if (title.isEmpty() == false) {
            duri.setAttribute ("title", title);
        }
        
        return duri.toString();
    }

    // Example 2
    //-----------
    // Resolve link from data URI
    //
    // Input param dataUriString: string presentation of data URI, eg. 
    //  "data:txt/x-url;title=MeeGo%20Homepage,http%3A%2F%2Fwww.meego.com"
    //
    // Return value: Link (eq. "http://www.meego.com") or empty string if content
    //  isn't valid data uri with valid data mime type.
    //
    static QString getLinkUrlFromDataUri (const QString & dataUriString) {
        MDataUri duri = dataUriString;
        if (duri.isValid() == false) {
            qWarning() << "Invalid data uri input";
            return QString();
        }
        
        if (duri.isOfMimeType ("text/x-uri") == false) {
            qWarning() << "Don't know what to do with" << duri.mimeType();
            return QString();
        }
        
        if (duri.hasAttribute("title") == true) {
            qDebug() << "Found link with title:" << duri.attribute ("title");
        }
        
        return duri.textData();
    }

    // Example 3
    //-----------
    // Generate data URI from binary data and it's mime type
    //
    static QString binaryDataToDataURI (const QByteArray & data,
        const QString & mime) {
        
        MDataUri duri;
        duri.setBinaryData (data);
        duri.setMimeType (mime);
        return duri.toString();
    }

    // Example 4
    //-----------
    // Read binary data inside data URI
    //
    static QByteArray getBinaryDataFromDataUri (const QString & dataUriString) {
        MDataUri duri = dataUriString;
        if (duri.isValid() == false) {
            qWarning() << "Invalid data uri input";
            return QByteArray();
        }
        
        return duri.data();
    }

};

#endif
