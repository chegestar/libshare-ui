
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




#ifndef _SHARE_UI_DEFAULTS_PLUGIN_H_
#define _SHARE_UI_DEFAULTS_PLUGIN_H_

#include <QObject>
#include <ShareUI/PluginBase>

/*!
   \class DefaultsPlugin
   \brief Plugin class for ShareUI to provide default methods
   \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
  */
class DefaultsPlugin : public ShareUI::PluginBase {

Q_OBJECT    

public:
    
    /*!
      \brief Constructor
      \param parent QObject parent
     */
    DefaultsPlugin (QObject * parent = 0);
    
    virtual ~DefaultsPlugin ();
    
    /*!
      \brief See ShareUI::MethodPluginInterface
      \return Will return methods for email and bluetooth
     */
    QList <ShareUI::MethodBase *> methods (QObject * parent = 0);
};

#endif
