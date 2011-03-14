 
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




#ifndef _SUI_SERVICE_H_
#define _SUI_SERVICE_H_

#include <QObject>
#include <QStringList>
#include <QList>
#include <QApplication>
#include <ShareWidgets/UiLoader>

/*!
    \class Service
    \brief Service class for ShareUI
    \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
 */
class Service : public QObject 
{
Q_OBJECT

public:
    Service(QObject *parent = 0);
    virtual ~Service();

    /*!
      \brief Function that loads the plugin providing the actual sharing UI
             and returns pointer to the QApplication instance for the
             application
      \param argc Count of number of arguments passed
      \param argv List of arguments passed to the application
      \return QApplication pointer
     */
    QApplication * application (int & argc, char **argv);

public Q_SLOTS: // METHODS

    void share(const QStringList &fileList);

    /*!
      \brief Slot to used to kill application when started
     */
    void forceShutdownApp ();
    
private:

    ShareWidgets::UiLoader m_uiLoader; //!< UI loader
};

#endif
