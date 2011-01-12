
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

#include "ShareWidgets/PresentationWidget"
#include "presentationwidgetprivate.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(ShareWidgets::PresentationWidget);
#include <MLibrary>
M_LIBRARY

using namespace ShareWidgets;

PresentationWidget::PresentationWidget (ShareUI::ItemContainer * container,
    QGraphicsItem * parent) : MWidget (parent),
    d_ptr (new PresentationWidgetPrivate (container)) {
}

PresentationWidget::~PresentationWidget () {
    delete d_ptr;
}

ShareUI::ItemContainer * PresentationWidget::itemContainer () const {
    return d_ptr->m_itemContainer;
}

// --- private class -----------------------------------------------------------

PresentationWidgetPrivate::PresentationWidgetPrivate (
    ShareUI::ItemContainer * container) : m_itemContainer (container) {
}

PresentationWidgetPrivate::~PresentationWidgetPrivate() {
}