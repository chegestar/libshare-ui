#include "ShareUITests.h"
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



#include "ShareUI/Item"
#include "ShareUI/FileItem"
#include "ShareUI/DataUriItem"
#include "ShareUI/ItemContainer"
#include "ShareUI/pluginloader.h"
#include "bluetoothmethod.h"
#include "emailmethod.h"
#include "MDataUri"
#include <QtTest/QtTest>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <QGraphicsLinearLayout>

#define TEST_FILEPATH "/usr/share/libshare-ui-tests/image.jpg"

using namespace ShareUI;

void ShareUITests::initTestCase() {
       
    testFilePath = QDir::homePath() + "/MyDocs/.images/libshare-ui-test.jpg";
        
}

void ShareUITests::cleanupTestCase() {
}

void ShareUITests::testFile() {

    QVERIFY (QFile::exists (testFilePath));

    SharedItem noItem = FileItem::create("/tmp/this/folder/does/not/exist");
    QVERIFY(noItem.isNull() == true);

    SharedItem item = FileItem::create(QString("file://") + testFilePath);
    QVERIFY(item.isNull() == false);
    FileItem *fileItem = FileItem::toFileItem(item);
    QVERIFY(fileItem != 0);
    QCOMPARE(fileItem->size(), (quint64)9964);
    QCOMPARE(fileItem->mimeType(), QString("image/jpeg"));
    QVERIFY(fileItem->lastModified().isValid());
}

void ShareUITests::testContainer() {

    QVERIFY (QFile::exists (testFilePath));

    ShareUI::ItemContainer cont1;
    QCOMPARE (cont1.count(), 0);
    QVERIFY (cont1.hasSpace());
    
    /* Has to be rewritten
    QCOMPARE (cont1.byteSize(), (unsigned long long) 0);
    
    QString testFileURI = "file://" + testFilePath;
    
    ShareUI::FileContainer smallCont (1);    
    QVERIFY (smallCont.hasSpace());    
    QVERIFY (smallCont.addFile (testFileURI));
    //TODO: Correct size...
    QCOMPARE (smallCont.size(), 1);
    QCOMPARE (smallCont.byteSize(), (unsigned long long) 9964);
    QVERIFY (!smallCont.hasSpace());    

    // Let's use same path, should return true even that container is full
    QVERIFY (smallCont.addFile (testFileURI));
    //TODO: Correct size...
    QCOMPARE (smallCont.size(), 1);
    QCOMPARE (smallCont.byteSize(), (unsigned long long) 9964);
    QVERIFY (!smallCont.hasSpace());  

    ShareUI::ItemContainer cont;
    QStringList list;
    list << testFileURI;
    cont.appendItems (list);
    QCOMPARE (cont.count(), 1);
    */

}

#if 0
// Commenting this test case till bug 171199 is resolved
void ShareUITests::testLoader() {
    PluginLoader * loader = new PluginLoader (this);
    
    // Has default path
    QString defPath = loader->getPluginPath ();
    QVERIFY (!defPath.isEmpty());
    
    // Test path set (without plugins)
    loader->setPluginPath("/tmp");
    QCOMPARE (loader->getPluginPath(), QString ("/tmp"));
    
    // Test with no plugin loaded
    QCOMPARE (loader->pluginCount (), 0);
    QCOMPARE (loader->pluginName (0), QString());
    QCOMPARE (loader->pluginName (100), QString());    
    QVERIFY (loader->method (0) == 0);
    QVERIFY (loader->method (100) == 0); 
    
    QList <ShareUI::MethodBase *> methods = loader->methodsWithType (
        ShareUI::MethodBase::TYPE_SHARE_TO_SERVICE);
    QCOMPARE (methods.count(), 0);
    methods = loader->methodsWithType (ShareUI::MethodBase::TYPE_SHARE_VIA);
    QCOMPARE (methods.count(), 0);

    // Now try to load plugins from this path. There should be none loaded.
    QVERIFY(!loader->loadPlugins ());
    
    // Load bluetooth plugin as test plugin (this can be only run after install)
    loader->setPluginPath (defPath);
    QVERIFY(loader->loadPlugins ());
    // There should be at least our plugins
    QVERIFY (loader->pluginCount () >= 2);
    
    loader->unload ();     
    
    loader->setPluginPath (defPath);
    QVERIFY(loader->loadPlugins ());
    // Confirm it does not load the same plugin twice.
    QVERIFY(!loader->loadPlugins ());

    delete loader;
}
#endif

void ShareUITests::pluginTests() {

    QVERIFY (QFile::exists (testFilePath));
    ShareUI::ItemContainer iCont;
    QString testFileURI = "file://" + testFilePath;
    iCont.appendItem (testFileURI);
    QCOMPARE (iCont.count(), 1);  

    EmailMethod * email = new EmailMethod ();
    QVERIFY(!(email->title().isEmpty()));
    QString sub = email->subtitle();
    QString ico = email->icon();
    ShareUI::MethodBase::Type type = ShareUI::MethodBase::TYPE_OTHER;
    QVERIFY (email->type() == type);
    QCOMPARE (email->id(), QString("com.meego.email"));
    
    email->currentItems (&iCont);    
    delete email;
    
    BluetoothMethod * bluetooth = new BluetoothMethod ();
    QVERIFY(!(bluetooth->title().isEmpty()));
    sub = bluetooth->subtitle();
    ico = bluetooth->icon();
    QVERIFY (bluetooth->type() == type);
    QCOMPARE (bluetooth->id(), QString("com.meego.bluetooth"));

    bluetooth->currentItems (&iCont);
    delete bluetooth;
}

void ShareUITests::testDataURIAttributes() {
    MDataUri dataUri;
    //construct
    dataUri.setMimeType("application/x-pdf");
    dataUri.setAttribute("Title","HelloWorld");
    dataUri.setAttribute("Description","All is well");
    dataUri.setAttribute("Key With Space","Keywithspace");
    dataUri.setAttribute("ValueWithSpace","value with space");
    dataUri.setTextData("http://www.paper.com","utf-8");

    QCOMPARE(dataUri.toString(), QString(
        "data:application/x-pdf;Description=All%20is%20well;Key%20With%20" \
        "Space=Keywithspace;Title=HelloWorld;ValueWithSpace=value%20with%20" \
        "space;charset=utf-8,http%3A%2F%2Fwww.paper.com"));
    
    MDataUri comDataUri(dataUri.toString());
    QVERIFY(comDataUri.isValid());
    QCOMPARE(comDataUri.attribute("Title"), QString("HelloWorld"));
    QCOMPARE(comDataUri.attribute("Description"), QString("All is well"));
    QVERIFY(comDataUri.hasAttribute("Title"));
    QVERIFY(!comDataUri.hasAttribute("KeyWithSpace"));
    QCOMPARE(comDataUri.attribute("Key With Space"), QString("Keywithspace"));
    QCOMPARE(comDataUri.attribute("ValueWithSpace"), QString("value with space"));

    QMap<QString, QString> attributes;
    QMapIterator<QString, QString> iter = comDataUri.attributeIterator();
    while (iter.hasNext()) {
        iter.next();
        attributes.insert(iter.key(), iter.value());
    }
    QCOMPARE(attributes.count(), 4);
    QCOMPARE(attributes.value("Title"), QString("HelloWorld"));
    QCOMPARE(attributes.value("Description"), QString("All is well"));
    QCOMPARE(attributes.value("Key With Space"), QString("Keywithspace"));
    QCOMPARE(attributes.value("ValueWithSpace"), QString("value with space"));
    
    MDataUri noAttributeUri;
    noAttributeUri.setMimeType("application/x-pdf");
    QMapIterator<QString,QString> iter2 = noAttributeUri.attributeIterator();
    QVERIFY(iter2.hasNext() == false);
    
    MDataUri duri("data:title=meego%20is%20great;description=goodwork,http://www.meego.com");
    QVERIFY(duri.isValid());
    QCOMPARE(duri.attribute("title"), QString("meego is great"));
    QCOMPARE(duri.attribute("description"), QString("goodwork"));
    QCOMPARE(duri.textData(), QString("http://www.meego.com"));
    
    QString goUrl = "data:application/x-avalon;title=Link,http%3A%2F%2Fwww.google.com";
    duri = goUrl;
    QVERIFY(duri.isValid());
    QCOMPARE(duri.mimeType(), QString("application/x-avalon"));
    QVERIFY(duri == goUrl);
    QCOMPARE(duri.textData(), QString ("http://www.google.com"));
    
    goUrl = "data:application/x-avalon,http%3A%2F%2Fwww.google.com";
    duri = goUrl;
    QVERIFY(duri.isValid());
    QCOMPARE(duri.mimeType(), QString("application/x-avalon"));
    QVERIFY(duri == goUrl);
    QCOMPARE(duri.textData(), QString ("http://www.google.com"));
    
    MDataUri newUri;
    QString link = "http://www.meego.com";
    newUri.setMimeType ("text/x-uri");
    newUri.setTextData (link);
    newUri.setAttribute ("title", QString("MeeGo !!!!"));
    newUri.setAttribute ("description", QString("Jeeeess ssss"));
    QVERIFY(newUri.isValid());
    QCOMPARE(newUri.attribute("title"),QString("MeeGo !!!!"));
    QCOMPARE(newUri.textData(), link);
}

void ShareUITests::testDataURIData() {

    MDataUri dataUri;
    dataUri.setMimeType("application/x-pdf");
    dataUri.setAttribute("title", "MyTitle");

    const QString UTF8("utf-8");
    const QString UTF8_STRING("hääyöaie");
    dataUri.setTextData(UTF8_STRING, UTF8);
    QCOMPARE(dataUri.textData(), UTF8_STRING);
    QVERIFY(dataUri.data().isEmpty());
    
    const QString ASCII_STRING("http://www.paper.com");
    dataUri.setTextData(ASCII_STRING);
    QCOMPARE(dataUri.textData(), ASCII_STRING);
    QVERIFY(dataUri.data().isEmpty());

    QByteArray dataArray("http://www.paper.com");
    dataUri.setBinaryData(dataArray);
    QCOMPARE(dataUri.data(), dataArray);
    QVERIFY(dataUri.textData().isEmpty());
    QString dataUriText = dataUri.toString();
    QCOMPARE(dataUriText,
        QString("data:application/x-pdf;title=MyTitle;base64,aHR0cDovL3d3dy5wYXBlci5jb20="));
    MDataUri dataUri2(dataUriText);
    QCOMPARE(dataUri2.data(), dataArray);
    QCOMPARE(dataUri2.attribute("title"), QString("MyTitle"));

    QCOMPARE(MDataUri("data:simple").textData(), QString("simple"));
    QCOMPARE(MDataUri("data:,simple").textData(), QString("simple"));

}


void ShareUITests::testDataURIMimeType() {
    MDataUri mimeTypeDataUri;
    QVERIFY(mimeTypeDataUri.isValid() == false);
    mimeTypeDataUri.setTextData("http://www.paper.com","utf-8");
    QCOMPARE(mimeTypeDataUri.mimeType(), QString("text/plain"));
    QVERIFY(mimeTypeDataUri.isOfMimeType("text/plain"));
    mimeTypeDataUri.setMimeType("application/x-pdf");
    QCOMPARE(mimeTypeDataUri.mimeType(), QString("application/x-pdf"));
    QVERIFY(mimeTypeDataUri.isOfMimeType("application/x-pdf"));
    QVERIFY(!mimeTypeDataUri.isOfMimeType("text/plain"));
    
    MDataUri comDataUri(mimeTypeDataUri.toString());
    QCOMPARE(comDataUri.mimeType(),QString("application/x-pdf"));
    
}

void ShareUITests::testValidDataURI() {
    MDataUri validateDURI;
    QVERIFY(validateDURI.isValid() == false);
    validateDURI.read("http://www.google.com");
    QVERIFY(validateDURI.isValid() == false);
    validateDURI.read("data://www.google.com");
    QVERIFY(validateDURI.isValid() == false);
    validateDURI.read("data:someRealData");
    QVERIFY(validateDURI.isValid() == true);
    QCOMPARE(MDataUri("data:,thisisdata").isValid(), true);

    QVERIFY(validateDURI.read("dat:someRealData") == false);
    
    //This valid, read doesn't replace if failed
    QVERIFY(validateDURI.isValid() == true);
    validateDURI = QString ("dat:someRealData");
    QVERIFY(validateDURI.isValid() == false);    

    QVERIFY(validateDURI.read("data:") == false);

    QVERIFY (validateDURI.read("data") == false);

    QCOMPARE(MDataUri("data:text/x-url;title=Hello%20world;description=Foo%20Bar"
        ";thisisnotdata").isValid(), false);

    QCOMPARE(MDataUri("data:text/x-url;title=Hello%20world;description=Foo%20Bar"
        ",thisisdata").isValid(), true);

}

void ShareUITests::testDataURIEquality()
{
    MDataUri dataUri("data:text/x-url;title=hello there;description=world,"
        "http://www.helloworld.com");
    MDataUri cmpDataUri("data:text/x-url;charset=us-ascii;description=world;"
        "title=hello there,http://www.helloworld.com");

    MDataUri dataUri2("data:text/plain;charset=utf8;title=hello there;description=world,"
        "http://www.helloworld.com");
    QString cmpDataUri2("data:description=world;charset=UTF8;"
        "title=hello there,http%3A%2F%2Fwww.helloworld.com");
    QUrl cmpDataUri3("data:description=world;charset=UTF8;"
        "title=hello there,http://www.helloworld.com");

    QVERIFY(dataUri == cmpDataUri);
    QVERIFY(dataUri2 == cmpDataUri2);
    QVERIFY(dataUri2 == cmpDataUri3);
}

/*********
 *
 * Commenting for now
void ShareUITests::testDataURIFile() {
    MDataUri dataUri;
    dataUri.setMimeType("text/x-vcard");
    dataUri.setAttribute ("title", "Mauno Ahonen");
    
    QString text = QLatin1String ("Hello World\nFoo Bar\nLorem Ipsum");
    QByteArray data = text.toAscii();
    
    dataUri.setTextData (text);
    
    QString path = QDir::tempPath() + QDir::separator() +
        QLatin1String("mdatauri.data");
    
    QVERIFY (dataUri.writeDataToFile (path));
    QVERIFY (QFile::exists (path));
    QVERIFY (dataUri.writeDataToFile (path));
    QVERIFY (QFile::exists (path));

    QFile file (path);
    QVERIFY (file.open (QIODevice::ReadOnly));
    QByteArray fileData = file.QIODevice::readAll();
    file.close();
    
    QCOMPARE (data, fileData);
    
    // Binary write
    data.clear();
    for (char i = 0; i != -1; i++) {
        data.append (i);
    }
    dataUri.setBinaryData (data);    
    
    QVERIFY (dataUri.writeDataToFile (path));
    QVERIFY (file.open (QIODevice::ReadOnly));
    fileData = file.QIODevice::readAll();
    file.close();    

    QCOMPARE (dataUri.data(), fileData);    
    QCOMPARE (data, fileData);
    
    // Empty data
    dataUri.setBinaryData (QByteArray());
    QVERIFY (dataUri.writeDataToFile (path));
    QVERIFY (file.open (QIODevice::ReadOnly));
    fileData = file.QIODevice::readAll();
    file.close();    
    
    QCOMPARE (QByteArray(), fileData);    
    
    QVERIFY (QFile::remove(path));
}

***********/

void ShareUITests::testDataUriItem() {
   
    QStringList inputList;
    inputList << "http://not.valid.com"; //not valid
    inputList << "data:text/x-url,http%3A%2F%2Fis.valid.com"; //valid
    inputList << "uid:23423423-234234-2342344-234"; //not valid
    inputList << "data:text/avalon;title=Hello%20World;description=Foo%20Bar,http%3A%2F%2Fis.valid.com"; //valid
    inputList << ""; //not valid
    inputList << "data%20plan%20is%20not"; //not valid
    inputList << "file:///usr/bin/echo"; //not valid
    inputList << "/usr/bin/echo"; //not valid
    
    QList<ShareUI::SharedItem > items = ShareUI::DataUriItem::createList (
        inputList);
    
    QCOMPARE (items.count(), 2);
    
    ShareUI::DataUriItem * dItem = ShareUI::DataUriItem::toDataUriItem (
        items.at (0));
    QVERIFY (dItem != 0);
    QCOMPARE (dItem->title(), QString("http://is.valid.com"));
    QVERIFY (dItem->description().isEmpty());
    QCOMPARE (dItem->mimeType(), QString("text/x-url"));
    QCOMPARE (dItem->constructInfo(), inputList.at(1));

    dItem = ShareUI::DataUriItem::toDataUriItem (items.at (1));
    QVERIFY (dItem != 0);
    QCOMPARE (dItem->title(), QString("Hello World"));
    QCOMPARE (dItem->description(), QString("Foo Bar"));  
    QCOMPARE (dItem->mimeType(), QString("text/avalon"));
    QCOMPARE (dItem->constructInfo(), inputList.at(3));
    
    //TODO: icon test
}

QTEST_MAIN(ShareUITests)