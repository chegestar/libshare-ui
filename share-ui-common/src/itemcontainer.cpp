 
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

#include "ShareUI/ItemContainer"
#include "itemcontainer_p.h"
#include <QDebug>
#include <ShareUI/FileItem>
#include <ShareUI/DataUriItem>
#include <QListIterator>

using namespace ShareUI;

ItemContainer::ItemContainer (int sizeLimit, QObject * parent) :
    QObject (parent), d_ptr (new ItemContainerPrivate(this, sizeLimit)) {
    
    connect (d_ptr, SIGNAL (itemsReady()), this, SIGNAL (itemsReady()),
        Qt::QueuedConnection);
}


ItemContainer::~ItemContainer () {
    qDebug() << "ItemContainer destructor" << this;
    d_ptr->disconnect (this);
    delete d_ptr;
}

int ItemContainer::count () const {
    return d_ptr->m_items.count();
}

bool ItemContainer::isEmpty() const {
    return d_ptr->m_items.isEmpty();
}

bool ItemContainer::hasSpace() const {
    return !(d_ptr->m_sizeLimit != 0 &&
        d_ptr->m_sizeLimit <= d_ptr->m_items.count());
}

QSharedPointer <ShareUI::Item> ItemContainer::getItem (int index) const {
    return d_ptr->m_items.at (index);
}

QListIterator<QSharedPointer <ShareUI::Item> > ItemContainer::itemIterator()
    const {
    
    return QListIterator<QSharedPointer <ShareUI::Item> >(d_ptr->m_items);
}

void ItemContainer::appendItems (const QStringList & constructInfo) {
    if (d_ptr->appendItems (constructInfo) == true) {
        Q_EMIT (changed());
    }
}

void ItemContainer::appendItem (const QString & constructInfo) {
    appendItems (QStringList() << constructInfo);
}

bool ItemContainer::isReady () const {
    return d_ptr->m_waitingItems.isEmpty();
}

int ItemContainer::waitingItemsCount () const {
    return d_ptr->m_waitingItems.size();
}

quint64 ItemContainer::totalSize() const {
    if (isReady() == false) {
        return 0;
    }

    quint64 ret = 0;
    ItemIterator i = itemIterator();
    while (i.hasNext() == true) {
        SharedItem item = i.next();
        ret += item->size();
    }
    return ret;
}

// --- private class -----------------------------------------------------------
ItemContainerPrivate::ItemContainerPrivate (ItemContainer * parent,
    int sizeLimit) : QObject (parent), m_sizeLimit (sizeLimit) {
}

ItemContainerPrivate::~ItemContainerPrivate() {
}

bool ItemContainerPrivate::appendItems (const QStringList & constructInfo) {

    int itemsAtStart = m_items.count();

    QStringList ownList = constructInfo;

    QList<QSharedPointer<Item> > items = DataUriItem::createList (
        constructInfo);
    cleanList (ownList, items);
    m_items += items; 
    items.clear();
        
    items = FileItem::createList (ownList);
    cleanList (ownList, items);
    m_items += items;
    items.clear();
        
    // Add to waiting list...
    for (int i = 0; i < m_items.count(); ++i) {
        Item * item = m_items.at(i).data();
        if (item->isReady() == false) {
            m_waitingItems.append (item);
            connect (item, SIGNAL (ready()), this, SLOT(itemReady()));
        }
    }
        
    if (itemsAtStart >= m_items.count()) {
        return false;
    } else {
        return true;
    }
}

void ItemContainerPrivate::cleanList (QStringList & list,
    QList<QSharedPointer<Item> > & find) {
    
    QListIterator <QSharedPointer<Item> > iter (find);
    
    while (iter.hasNext()) {
        list.removeAll (iter.next()->constructInfo());
    }   
}

void ItemContainerPrivate::itemReady () {
    Item * item = qobject_cast<Item*> (sender());
   
    if (item == 0) {
        qCritical() << "Invalid caller for " << __FUNCTION__;
        return;
    }
    
    qDebug() << "Item container received item ready from" << item;
    m_waitingItems.removeOne (item);
    
    if (m_waitingItems.isEmpty() == true) {
        qDebug() << "All items ready, emitting container ready";
        emit (itemsReady());
    }
}
