
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

#include <ShareUI/Item>
#include <ShareUI/DataUriItem>
#include <QTimer>
#include "datauriitem_p.h"

using namespace ShareUI;
typedef DataUriItemPrivate MyPrivate;

SharedItem DataUriItem::create (const QString & dataUri) {
    DataUriItem * aItem = new DataUriItem (dataUri);
    SharedItem item (aItem);
    
    MDataUri & uri = aItem->d_ptr->m_dataUri;
    aItem = 0;
    
    if (uri.read (dataUri) == false) {
        item.clear();
    }
    
    return item;
}

QList<SharedItem > DataUriItem::createList (const QStringList & dataUris) {
    QList<QSharedPointer<Item> > list;
    
    for (int i = 0; i < dataUris.count(); ++i) {
        QSharedPointer<Item> item = DataUriItem::create (dataUris.at (i));
        if (item.isNull() == false) {
            list.append (item);
        }
    }
    
    return list;
}

DataUriItem::DataUriItem (const QString & constructInfo, QObject * parent) :
    Item (constructInfo, parent), d_ptr (new MyPrivate()) {
    
    QTimer::singleShot (10000, this, SIGNAL (ready()));
}

DataUriItem::~DataUriItem() {
    delete d_ptr;
}

QString DataUriItem::title() {
    QString myTitle = d_ptr->m_dataUri.attribute ("title");
    if (myTitle.isEmpty() == true) {
        myTitle = d_ptr->m_dataUri.textData();
    }
    return myTitle;
}

QString DataUriItem::description() {
    return d_ptr->m_dataUri.attribute ("description");
}

QString DataUriItem::icon() {

    QString icon;

    if (mimeType() == "text/x-vcard") {
        icon = QLatin1String("icon-m-content-avatar-placeholder");
    } else {
        icon = QLatin1String("icon-m-content-url");
    }

    QString iconAttr = d_ptr->m_dataUri.attribute ("preview");
    
    if (iconAttr.isEmpty() == false) {
        if (iconAttr.startsWith("base64,") == false) {
            qDebug() << "DataURI preview attribute:" << iconAttr;
            icon = iconAttr;
        } else {
            qDebug() << "DataURI preview attribute is base64";
        }
    }
    
    return icon;
}

QString DataUriItem::toString() {
    QString output = d_ptr->m_dataUri.attribute ("title");

    QString desc = description();
    if (desc.isEmpty() == false) {
    
        if (output.isEmpty() == false) {
            output.append (", ");
        }
        output.append (desc);    
    }

    if (output.isEmpty() == false) {
        output.append (", ");
    }
    
    output.append (d_ptr->m_dataUri.textData());
    
    return output;
}

QString DataUriItem::mimeType() {
    return d_ptr->m_dataUri.mimeType();
}

const MDataUri & DataUriItem::dataUri() const {
    return d_ptr->m_dataUri;
}

void DataUriItem::loadThumbnail (QSize size) {
    //TODO: Make nonblocking?
    
    QString iconAttr = d_ptr->m_dataUri.attribute ("thumbnail");
    //TODO: Only one attribute name should be used
    if (iconAttr.isEmpty() == true) {
        iconAttr = d_ptr->m_dataUri.attribute ("previewThumbnail");    
    }    
    
    if (iconAttr.startsWith("base64,") == false) {
        return;
    }
    
    //TODO: Use refs and tricks to avoid one extra string copy here
    QByteArray b64Data = iconAttr.mid (7).toAscii();
    QByteArray data = QByteArray::fromBase64 (b64Data);

    QPixmap map (size);
    if (map.loadFromData (data) == true) {
        emit (thumbnail (map));
    }
}

// -- private ----------------------------------------------------------------

DataUriItemPrivate::DataUriItemPrivate () {
}

DataUriItemPrivate::~DataUriItemPrivate () {
}