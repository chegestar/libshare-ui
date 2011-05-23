
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

#include <ShareWidgets/SharedMethodData>
#include "sharedmethoddataprivate.h"

using namespace ShareWidgets;

SharedMethodData * SharedMethodDataPrivate::m_singleton = 0;

SharedMethodData * SharedMethodData::instance() {
    if (SharedMethodDataPrivate::m_singleton == 0) {
        SharedMethodDataPrivate::m_singleton = new SharedMethodData (0);
    }

    return SharedMethodDataPrivate::m_singleton;
}

SharedMethodData::SharedMethodData (QObject * parent) : QObject (parent),
    d_ptr (new SharedMethodDataPrivate (this)) {
}

SharedMethodData::~SharedMethodData () {
    delete d_ptr;
}

ShareWidgets::ApplicationViewInterface * SharedMethodData::applicationView (
    ShareUI::MethodBase * caller) const {
    
    ShareWidgets::ApplicationViewInterface * ret = 0;
    
    QHash<ShareUI::MethodBase*, MethodsData>::const_iterator i =
        d_ptr->m_methodMap.find(caller);
    if (i != d_ptr->m_methodMap.end()) {
        ret = i.value().m_view;
    }         

    return ret;
}


void SharedMethodData::registerMethod (ShareUI::MethodBase * method,
    ShareWidgets::ApplicationViewInterface * view) {
        
    QHash<ShareUI::MethodBase*, MethodsData>::iterator i =
        d_ptr->m_methodMap.find(method);
    
    if (i != d_ptr->m_methodMap.end()) {
        i.value().m_view = view;
    } else {
        MethodsData data;
        data.m_view = view;
        d_ptr->m_methodMap.insert (method, data);
    }    
}

void SharedMethodData::unregisterMethod (ShareUI::MethodBase * method) {
    d_ptr->m_methodMap.remove (method);
}

// --- private class ----------------------------------------------------------

SharedMethodDataPrivate::SharedMethodDataPrivate (SharedMethodData * parent) :
    m_publicObject (parent) {

}

SharedMethodDataPrivate::~SharedMethodDataPrivate() {
}
