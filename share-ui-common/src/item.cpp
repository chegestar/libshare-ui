 
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

#include "ShareUI/item.h"
#include <QDebug>

using namespace ShareUI;

Item::Item (const QString & constructInfo, QObject * parent) : QObject (parent),
    m_constructInfo (constructInfo) {
}

Item::~Item () {
}

QString Item::description() {
    return QString();
}

QString Item::toString() {
    return QString();
}

QString Item::mimeType() {
    return QString();
}

QString Item::constructInfo () const {
    return m_constructInfo;
}

bool Item::isReady() {
    // Always ready
    return true;
}

void Item::loadThumbnail (QSize size) {
    // Does not do anything
    Q_UNUSED (size);
    return;
}

quint64 Item::size() const {
    return 0;
}
