
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

#include "ShareWidgets/ApplicationPage"
#include "applicationpageprivate.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(ShareWidgets::ApplicationPage);
#include <MLibrary>
M_LIBRARY

using namespace ShareWidgets;

ApplicationPage::ApplicationPage (ShareUI::PluginLoader * pluginLoader,
    ShareUI::ItemContainer * container, QGraphicsItem *parent) :
    MApplicationPage (parent), d_ptr (new ApplicationPagePrivate (pluginLoader,
    container)) {
}

ApplicationPage::~ApplicationPage () {
    delete d_ptr;
}

PresentationWidget * ApplicationPage::presentationWidget() const {
    return d_ptr->m_presentationWidget;
}

void ApplicationPage::setMainWidget (MWidget * widget) {
    d_ptr->m_pageWidget = widget;
}

MWidget * ApplicationPage::mainWidget() const {
    return d_ptr->m_pageWidget;
}

void ApplicationPage::setPresentationWidget (PresentationWidget * widget) {
    d_ptr->m_presentationWidget = widget;
}

ShareUI::PluginLoader * ApplicationPage::pluginLoader () const {
    return d_ptr->m_pluginLoader;    
}

ShareUI::ItemContainer * ApplicationPage::itemContainer() const {
    return d_ptr->m_itemContainer;
}

// --- private class functions -------------------------------------------------

ApplicationPagePrivate::ApplicationPagePrivate (ShareUI::PluginLoader * loader,
    ShareUI::ItemContainer * container) : m_pluginLoader (loader),
    m_itemContainer (container), m_presentationWidget (0), m_pageWidget (0) {
}

ApplicationPagePrivate::~ApplicationPagePrivate () {
}
