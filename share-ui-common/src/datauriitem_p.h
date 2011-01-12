 
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

#ifndef _SHARE_UI_DATA_URI_ITEM_PRIVATE_H_
#define _SHARE_UI_DATA_URI_ITEM_PRIVATE_H_

#include <MDataUri>

namespace ShareUI {

    /*!
      \class DataUriItemPrivate
      \brief Private class for DataUriItem
      \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class DataUriItemPrivate {
    
    public:
    
        /*!
          \brief Constructor
          \param sizeLimit Size limit of container. 0 == no size limit.
         */
        DataUriItemPrivate ();
        virtual ~DataUriItemPrivate();
        
        MDataUri m_dataUri; //!< Data URI
    };
}

#endif