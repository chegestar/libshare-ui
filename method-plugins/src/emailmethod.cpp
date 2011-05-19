 
/*
 * This file is part of Handset UX Share user interface
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 * Contact: Jukka Tiihonen <jukka.tiihonen@nokia.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,     
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  
 * IN THE SOFTWARE. 
 */




#include "emailmethod.h"
#include <QDebug>
#include <QProcess>
#include <ShareUI/ItemContainer>
#include <ShareUI/DataUriItem>
#include <ShareUI/FileItem>
#include "methodscommon.h"

using namespace ShareUiDefaultMethods;

EmailMethod::EmailMethod (QObject * parent) : ShareUI::MethodBase (parent) {
}

EmailMethod::~EmailMethod () {
}

QString EmailMethod::title () {
    //% "Email"
    QString name = qtTrId ("qtn_tui_share_via_email");
    return name;
}

QString EmailMethod::icon () {
    return QString::fromLatin1 ("icon-m-content-email");
}

QString EmailMethod::id () {
    return QString::fromLatin1 ("com.meego.email");
}

void EmailMethod::selected (const ShareUI::ItemContainer * items) {
    
    QStringList filePaths;
    QString body;
    QString subject;
    
    ShareUI::ItemIterator itemsIter = items->itemIterator();
    while (itemsIter.hasNext()) {
    
        ShareUI::SharedItem item = itemsIter.next();
        ShareUI::DataUriItem * uriItem =
            ShareUI::DataUriItem::toDataUriItem (item);
                    
        if (uriItem != 0) {
        
            const MDataUri & dataUri = uriItem->dataUri();
            bool isVcard = dataUri.isOfMimeType (QLatin1String("text/x-vcard"));
    
            // Contact card has to be serialized to file
            if (isVcard == true) {
                QString path = makeFilePathForVCard (dataUri);

                if (dataUri.writeDataToFile (path) == true) {
                    filePaths << path;
                }
            
            // Present everything else in body of email
            } else {
                if (body.isEmpty() == false) {
                    body.append (" ");
                }        
                body.append (item->toString());
            }
            
            subject = uriItem->dataUri().attribute ("title");
        
        } else {          
            ShareUI::FileItem * fileItem = qobject_cast<ShareUI::FileItem*> (
                item.data());
                
            if (fileItem != 0) {
                filePaths << fileItem->filePath();
            }
        }
    
    }
    
    QStringList arguments;
    
    // Add subject
    if (subject.isEmpty() == false) {
        QString subjectArg (subject.replace ("\"", "\\\""));
        arguments << QString ("--subject");
        arguments << subjectArg;
    }
    
    // Add body
    if (body.isEmpty() == false) {
        QString bodyArg (body.replace ("\"", "\\\""));
        arguments << QString ("--body");
        arguments << bodyArg;
    }

    // Add files
    for (int i = 0; i < filePaths.count(); ++i) {
        arguments << QString ("--attach");
        arguments << filePaths.at (i);
    }
    
    qDebug() << "Calling /usr/bin/xdg-email with arguments"
        << arguments.join (" ");

    QProcess process;
    if (!QProcess::startDetached ("/usr/bin/xdg-email", arguments, "/tmp")) { 
    
        qCritical() << "Couldn't call xdg-email";
        emit (selectedFailed (QLatin1String("Failed to start xdg-email")));
    } else {
        // Emit back to SUI that we are done.
        emit (done());
    }
}

void EmailMethod::currentItems (const ShareUI::ItemContainer * items) {
    Q_EMIT (visible (acceptContent (items)));
}

bool EmailMethod::acceptContent (const ShareUI::ItemContainer * items) {
    // Safety check
    if (items == 0) {
        return false;
    }
    
    if (items->count() == 0) {
        return false;
    }

    ShareUI::ItemIterator itemsIter = items->itemIterator();

    // Limit to data uris and files
    while (itemsIter.hasNext()) {
    
        ShareUI::SharedItem item = itemsIter.next();
        
        ShareUI::DataUriItem * uriItem = ShareUI::DataUriItem::toDataUriItem (
            item);
        ShareUI::FileItem * fileItem = ShareUI::FileItem::toFileItem (item);
            
        if (uriItem == 0 && fileItem == 0) {
            return false;
        }
    }
    
    return true;
}
