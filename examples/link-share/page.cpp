
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




#include "page.h"
#include <QDebug>
#include <MButton>
#include <MLinearLayoutPolicy>
#include <MLayout>
#include <MLabel>
#include <MTextEdit>
#include <MApplicationWindow>
#include <MDataUri>
#include <shareuiinterface.h>

Page::Page (QGraphicsItem *parent) : MApplicationPage (parent) {
    setTitle ("Link Share");
}


Page::~Page () {
}

void Page::addTextField (MLinearLayoutPolicy * policy,
    const QString & captionText, bool multiline) {
    
    MLabel * caption = new MLabel ();
    caption->setText (captionText);
    policy->addItem (caption);
    
    MTextEditModel::LineMode lineMode = MTextEditModel::SingleLine;
    if (multiline == true) {
        lineMode = MTextEditModel::MultiLine;
    }
    
    MTextEdit * edit = new MTextEdit (lineMode);
    edit->setInputMethodAutoCapitalizationEnabled (false);
        
    policy->addItem (edit);
    
    m_textedits.insert (captionText, edit);    
}

void Page::createContent () {
    MLayout * layout = new MLayout (centralWidget ());
    MLinearLayoutPolicy * policy = new MLinearLayoutPolicy (layout,
        Qt::Vertical);
        
    addTextField (policy, "link");
    addTextField (policy, "title");
    addTextField (policy, "description", true);    
            
    MButton * button = new MButton ();
    button->setText ("Share");    
    connect (button, SIGNAL (clicked()), this, SLOT (shareClicked()));
    policy->addItem (button);

    centralWidget ()->setLayout (layout);
}

void Page::shareClicked () {

    QString link = m_textedits.value("link")->text();
    QString title = m_textedits.value("title")->text();
    QString desc = m_textedits.value("description")->text();
    
    qDebug() << "Make data URI from" << link << title << desc;
    
    MDataUri duri;
    duri.setMimeType ("text/x-url");
    duri.setTextData (link);
    duri.setAttribute ("title", title);
    duri.setAttribute ("description", desc);
    
    if (duri.isValid() == false) {
        qCritical() << "Invalid URI";
        return;
    }
    
    QStringList items;
    items << duri.toString();
    qDebug() << "URI:" << items.join (" ");
    
    // Create a interface object
    ShareUiInterface shareIf;
    
    // You can check if interface is valid
    if (shareIf.isValid()) {
        // Start ShareUI application with selected files. 
        shareIf.share (items);
    } else {
        qCritical() << "Invalid interface";
        return;
    }
}