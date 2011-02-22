
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



 
#include "shareuiinterface.h"
#include <QStringList>
#include <MApplication>
#include <QDebug>

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    
    // Create a QStringlist where file uri will be stored
    QStringList uris; 
    
    // Append the file uris to the list (files need to indexed by Tracker)
    // If application is reading files from Tracker then Tracker's "file ID
    // URIs" should be used instead. These are URIs of files entry in Tracker
    // database. In this example simple file path URI is used to keep code
    // simple.
    uris << "file://tmp/fake.txt"; 
    
    // Create a interface object
    ShareUiInterface shareIf(ShareUiInterface::staticInterfaceName(), "/", QDBusConnection::systemBus(), &app);
    
    // You can check if interface is valid
    if (shareIf.isValid()) {
        // Start ShareUI application with selected files. 
        shareIf.share (uris);
    } else {
        qCritical() << "Invalid interface";
        return -1;
    }
        
    return app.exec();
}
