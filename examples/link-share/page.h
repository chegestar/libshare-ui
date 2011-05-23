
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




#ifndef _PAGE_H_
#define _PAGE_H_

#include <QObject>
#include <MApplicationPage>
#include <QMap>

class MTextEdit;
class MLinearLayoutPolicy;

class Page : public MApplicationPage {

Q_OBJECT 

public:
    Page (QGraphicsItem *parent = 0);
    virtual ~Page ();
    virtual void createContent ();
    
    // Add text edit field to layout policy
    void addTextField (MLinearLayoutPolicy * policy,
        const QString & captionText, bool multiline = false);

private Q_SLOTS:

    // Slot for send button
    void shareClicked ();

private:
    
    QMap <QString, MTextEdit*> m_textedits;
};

#endif
