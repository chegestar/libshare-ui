
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

#ifndef _SHARE_WIDGETS_APPLICATION_PAGE_PRIVATE_H_
#define _SHARE_WIDGETS_APPLICATION_PAGE_PRIVATE_H_

#include <ShareUI/ItemContainer>
#include <ShareUI/PluginLoader>

namespace ShareWidgets {
    class ApplicationPagePrivate {
    public:
        ApplicationPagePrivate (ShareUI::PluginLoader * loader,
            ShareUI::ItemContainer * container);
        virtual ~ApplicationPagePrivate();
        
        ShareUI::PluginLoader * m_pluginLoader; //!< Plugin loader
        ShareUI::ItemContainer * m_itemContainer; //!< Item container
        PresentationWidget * m_presentationWidget; //!< Presentation widget on page
        MWidget * m_pageWidget; //!< Page widget
    
    };
}
#endif
