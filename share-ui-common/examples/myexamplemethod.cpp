 
/*
 * This file is part of Handset UX Share user interface
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 * Contact: Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
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

/*!
 * \example myexamplemethod.cpp
 * Implementation of Example Share UI method
 */

#include "myexamplemethod.h"
#include <QDebug>
#include <QProcess>
#include <ShareUI/ItemContainer>
#include <ShareUI/DataUriItem>
#include <ShareUI/FileItem>

MyExampleMethod::MyExampleMethod (QObject * parent) :
    ShareUI::MethodBase (parent) {
}

MyExampleMethod::~MyExampleMethod () {
}

QString MyExampleMethod::title () {
    return QLatin1String ("My Example");
}

QString MyExampleMethod::icon () {
    return QString();
}

QString MyExampleMethod::id () {
    //Use unique id here. Like "com.nokia.email".
    return QString::fromLatin1 ("my.example");
}

void MyExampleMethod::selected (const ShareUI::ItemContainer * items) {

    //We do not do anything with content here. But you probably want to start
    //service with those or something else.
    Q_UNUSED (items);
    
    //Let's fake that we did start the sharing and identify it by emitting the
    //done signal. If done isn't emitted Share UI will stay open waiting for
    //it. So it should be emitted as soon as you have called you service or
    //started out-of-process parts that will do the actual sharing.
    Q_EMIT (done());
    
    return;
}

void MyExampleMethod::currentItems (const ShareUI::ItemContainer * items) {
    //Let's say for example we only accept jpeg images. So if given content
    //is anything else that jpegs, we should ask Share UI to hide this method.
    
    bool showMe = true;
    
    ShareUI::ItemIterator itemsIter = items->itemIterator();
    while (itemsIter.hasNext()) {
    
        ShareUI::SharedItem item = itemsIter.next();
        
        if (item->mimeType() != QLatin1String("image/jpeg")) {
            showMe = false;
            break;
        }
    }
    
    Q_EMIT (visible (showMe));
}
