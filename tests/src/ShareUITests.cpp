#include "ShareUITests.h"
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



#include "ShareUI/Item"
#include "ShareUI/FileItem"
#include "ShareUI/DataUriItem"
#include "ShareUI/ItemContainer"
#include "ShareUI/pluginloader.h"
#include "bluetoothmethod.h"
#include "emailmethod.h"
#include "MDataUri"
#include "sharedmethoddata.h"
#include "defaultsplugin.h"
#include <QtTest/QtTest>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <QGraphicsLinearLayout>
#include <QSignalSpy>

#define TEST_FILEPATH "/usr/share/libshare-ui-tests/image.jpg"

using namespace ShareUI;
using namespace ShareWidgets;

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
    QCOMPARE(fileItem->title(), QString("libshare-ui-test.jpg"));
    QCOMPARE(fileItem->description(), QString());
    QVERIFY(fileItem->icon().contains("icon"));
    QCOMPARE(fileItem->filePath(), testFilePath);
    QCOMPARE(fileItem->fileUri(), QString("file://") + testFilePath);
    QCOMPARE(fileItem->URI(), QUrl(QString("file://") + testFilePath));
    QVERIFY(!fileItem->trackerIri().isEmpty());
    QCOMPARE(fileItem->fileTitle(), QString("libshare-ui-test"));
    QCOMPARE(fileItem->fileDescription(), QString());
    QCOMPARE(fileItem->duration(), 0);
    QVERIFY(fileItem->lastModified().isValid() ||
        fileItem->contentCreated().isValid());


}

void ShareUITests::testContainer() {

    QVERIFY (QFile::exists (testFilePath));

    {
        // Container with max 2 items
        ItemContainer container(2);
        QCOMPARE(container.isEmpty(), true);
        QCOMPARE(container.hasSpace(), true);
        QCOMPARE(container.count(), 0);
        QCOMPARE(container.itemIterator().hasNext(), false);
        QCOMPARE(container.waitingItemsCount(), 0);
        QCOMPARE(container.totalSize(), (quint64)0);
        QCOMPARE(container.isReady(), true);

        // Append file item
        container.appendItem(QString("file://") + testFilePath);
        QCOMPARE(container.isEmpty(), false);
        QCOMPARE(container.hasSpace(), true);
        QCOMPARE(container.count(), 1);
        SharedItem item1 = container.getItem(0);
        FileItem *fileItem = FileItem::toFileItem(item1);
        QVERIFY(fileItem != 0);
        ItemIterator iterator1 = container.itemIterator();
        QCOMPARE(iterator1.hasNext(), true);
        SharedItem item2 = iterator1.next();
        QCOMPARE(iterator1.hasNext(), false);
        QCOMPARE(item1, item2);
        QCOMPARE(container.waitingItemsCount(), 0);
        QVERIFY(container.totalSize() > 0);
        QCOMPARE(container.isReady(), true);

        // Append data uri item
        container.appendItem(
            "data:text/x-url;title=Hello%20world;description=Foo%20Bar,thisisdata");
        QCOMPARE(container.isEmpty(), false);
        QCOMPARE(container.hasSpace(), false);
        QCOMPARE(container.count(), 2);
        SharedItem item3 = container.getItem(1);
        DataUriItem *dataUriItem = DataUriItem::toDataUriItem(item3);
        QVERIFY(dataUriItem != 0);
        ItemIterator iterator2 = container.itemIterator();
        QCOMPARE(iterator2.hasNext(), true);
        SharedItem item4 = iterator2.next();
        QCOMPARE(iterator2.hasNext(), true);
        QVERIFY(item3 != item4);
        SharedItem item5 = iterator2.next();
        QCOMPARE(item5, item3);
        QCOMPARE(iterator2.hasNext(), false);
        QCOMPARE(container.waitingItemsCount(), 0);
        QVERIFY(container.totalSize() > 0);
        QCOMPARE(container.isReady(), true);

    }

    {
        // Container without size limit
        ItemContainer container;
        QCOMPARE(container.isEmpty(), true);
        QCOMPARE(container.hasSpace(), true);
        QCOMPARE(container.count(), 0);
        QCOMPARE(container.itemIterator().hasNext(), false);
        QCOMPARE(container.waitingItemsCount(), 0);
        QCOMPARE(container.totalSize(), (quint64)0);
        QCOMPARE(container.isReady(), true);

        QSignalSpy changedSpy(&container, SIGNAL(changed()));

        // Append data uri item
        container.appendItem(
            "data:text/x-url;title=Hello%20world;description=Foo%20Bar,thisisdata");
        QCOMPARE(container.isEmpty(), false);
        QCOMPARE(container.hasSpace(), true);
        QCOMPARE(container.count(), 1);

        QCOMPARE(changedSpy.count(), 1);
    }

}

void ShareUITests::testLoader() {
    PluginLoader * loader = new PluginLoader (this);
    
    // Has default path
    QString defPath = loader->pluginPath ();
    QVERIFY (!defPath.isEmpty());
    
    // Test path set (without plugins)
    loader->setPluginPath("/tmp");
    QCOMPARE (loader->pluginPath(), QString ("/tmp"));
    
    // Test with no plugin loaded
    QCOMPARE (loader->pluginCount (), 0);
    QCOMPARE (loader->pluginName (0), QString());
    QCOMPARE (loader->pluginName (100), QString());    
    QVERIFY (loader->method (0) == 0);
    QVERIFY (loader->method (100) == 0); 
    QCOMPARE (loader->methods().count(), 0);

    // Now try to load plugins from this path. There should be none loaded.
    QVERIFY(loader->loadPlugins ());
    QCOMPARE (loader->pluginCount (), 0);

    delete loader;
}

void ShareUITests::pluginTests() {

    QVERIFY (QFile::exists (testFilePath));

    // Defaults plugin returns two methods (bluetooth and email)
    DefaultsPlugin defaultsPlugin;
    QList<ShareUI::MethodBase *> defaultMethods = defaultsPlugin.methods();
    // We should get 1 or more default plugins
    QVERIFY(defaultMethods.count() >= 1);
    qDeleteAll(defaultMethods);
    defaultMethods.clear();

    ShareUI::ItemContainer iCont;
    ShareUI::ItemContainer emptyContainer;
    QString testFileURI = "file://" + testFilePath;
    iCont.appendItem (testFileURI);
    QCOMPARE (iCont.count(), 1);  

    EmailMethod * email = new EmailMethod ();
    QSignalSpy emailSpy(email, SIGNAL(visible(bool)));
    QVERIFY(!(email->title().isEmpty()));
    QString sub = email->subtitle();
    QString ico = email->icon();
    ShareUI::MethodBase::Type type = ShareUI::MethodBase::TYPE_OTHER;
    QVERIFY (email->type() == type);
    QCOMPARE (email->id(), QString("com.meego.email"));

    // Null container emits visible signal with "false"
    email->currentItems (0);
    QCOMPARE(emailSpy.count(), 1);
    QCOMPARE(emailSpy.at(0).at(0).toBool(), false);

    // Empty container emits visible signal with "false"
    email->currentItems (&emptyContainer);
    QCOMPARE(emailSpy.count(), 2);
    QCOMPARE(emailSpy.at(1).at(0).toBool(), false);
    
    // Container with valid file item emits visible signal with "true"
    email->currentItems (&iCont);
    QCOMPARE(emailSpy.count(), 3);
    QCOMPARE(emailSpy.at(2).at(0).toBool(), true);

    delete email;
    

    BluetoothMethod * bluetooth = new BluetoothMethod ();
    QSignalSpy bluetoothSpy(bluetooth, SIGNAL(visible(bool)));
    QVERIFY(!(bluetooth->title().isEmpty()));
    sub = bluetooth->subtitle();
    ico = bluetooth->icon();
    QVERIFY (bluetooth->type() == type);
    QCOMPARE (bluetooth->id(), QString("com.meego.bluetooth"));

    // Null container emits visible signal with "false"
    bluetooth->currentItems (0);
    QCOMPARE(bluetoothSpy.count(), 1);
    QCOMPARE(bluetoothSpy.at(0).at(0).toBool(), false);

    // Empty container emits visible signal with "false"
    bluetooth->currentItems (&emptyContainer);
    QCOMPARE(bluetoothSpy.count(), 2);
    QCOMPARE(bluetoothSpy.at(1).at(0).toBool(), false);

    // Container with valid file item emits visible signal with "true"
    bluetooth->currentItems (&iCont);
    QCOMPARE(bluetoothSpy.count(), 3);
    QCOMPARE(bluetoothSpy.at(2).at(0).toBool(), true);

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

    QFile file (path);
    QVERIFY (file.open (QIODevice::ReadOnly));
    QByteArray fileData = file.QIODevice::readAll();
    file.close();
    
    QCOMPARE (data, fileData);
    
    // Binary write
    data.clear();
    for (char i = 0; i != 255; i++) {
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

void ShareUITests::testDataUriItem() {
   
    QStringList inputList;
    inputList << "http://not.valid.com"; //not valid
    inputList << "data:text/x-url;preview=icon-m-content-my,http%3A%2F%2Fis.valid.com"; //valid
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
    QCOMPARE (dItem->icon(), QString("icon-m-content-my"));

    dItem = ShareUI::DataUriItem::toDataUriItem (items.at (1));
    QVERIFY (dItem != 0);
    QCOMPARE (dItem->title(), QString("Hello World"));
    QCOMPARE (dItem->description(), QString("Foo Bar"));  
    QCOMPARE (dItem->mimeType(), QString("text/avalon"));
    QCOMPARE (dItem->constructInfo(), inputList.at(3));
    QVERIFY (dItem->icon().contains("icon"));
    
}

void ShareUITests::testSharedMethodData() {
    SharedMethodData *data = SharedMethodData::instance();

    QVERIFY(data != 0);
    ApplicationViewInterface *view1 = (ApplicationViewInterface*)1;
    ApplicationViewInterface *view2 = (ApplicationViewInterface*)2;
    MethodBase *method1 = (MethodBase*)3;
    MethodBase *method2 = (MethodBase*)4;

    QCOMPARE(data->applicationView(method1), (ApplicationViewInterface*)0);
    QCOMPARE(data->applicationView(method2), (ApplicationViewInterface*)0);

    data->registerMethod(method1, view1);
    QCOMPARE(data->applicationView(method1), view1);
    QCOMPARE(data->applicationView(method2), (ApplicationViewInterface*)0);

    data->registerMethod(method2, view2);
    QCOMPARE(data->applicationView(method1), view1);
    QCOMPARE(data->applicationView(method2), view2);

    data->unregisterMethod(method1);
    QCOMPARE(data->applicationView(method1), (ApplicationViewInterface*)0);
    QCOMPARE(data->applicationView(method2), view2);

    data->unregisterMethod(method2);
    QCOMPARE(data->applicationView(method1), (ApplicationViewInterface*)0);
    QCOMPARE(data->applicationView(method2), (ApplicationViewInterface*)0);

}

QTEST_MAIN(ShareUITests)
