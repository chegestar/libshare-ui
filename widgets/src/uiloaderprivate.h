
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

#ifndef _SHARE_WIDGETS_UI_LOADER_PRIVATE_H_
#define _SHARE_WIDGETS_UI_LOADER_PRIVATE_H_

namespace ShareWidgets {
    class UiLoader;
    class UiImplementationBase;

    /*!
      \class UiLoaderPrivate
      \brief Private class for UiLoader
     */
    class UiLoaderPrivate {
    public:
        UiLoaderPrivate (UiLoader * parent);
        virtual ~UiLoaderPrivate ();
        
        UiLoader * m_uiLoader; //!< Public instance
        UiImplementationBase * m_impl; //!< Pointer to implementation class
        QApplication * m_application; //!< Application received from plugin
    };
}
#endif
