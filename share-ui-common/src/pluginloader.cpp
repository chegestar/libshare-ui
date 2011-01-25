 
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

#include "ShareUI/pluginloader.h"
#include "pluginloader_p.h"
#include "ShareUI/PluginInterface"
#include "ShareUI/PluginBase"
#include <QDebug>
#include <QFileInfo>
#include <QRegExp>

// Folder where plugins are loaded
#define SHARE_UI_PLUGIN_FOLDER "/usr/lib/share-ui/plugins"
// Path to configuration file (method/plugin ordering)
#define SHARE_UI_CONF_PATH "/etc/share-ui.conf"
// Filter for plugins
#define SHARE_UI_PLUGIN_FILTER "lib*.so"

#define SHARE_UI_PLUGIN_CLEAN_PREFIX "lib"
#define SHARE_UI_PLUGIN_CLEAN_SUFFIX ".so"

using namespace ShareUI;

PluginLoader::PluginLoader(QObject * parent) : QObject (parent),
    d_ptr (new PluginLoaderPrivate (SHARE_UI_PLUGIN_FOLDER,
    SHARE_UI_CONF_PATH, this)) {
    
    connect (d_ptr, SIGNAL (newMethod(ShareUI::MethodBase*)), this,
        SIGNAL(newMethod(ShareUI::MethodBase*)));
        
    connect (d_ptr, SIGNAL (methodVisible(ShareUI::MethodBase*,bool)), this,
        SIGNAL(methodVisible(ShareUI::MethodBase*,bool)));
}

PluginLoader::~PluginLoader() {

    if (d_ptr->m_loaders.isEmpty() == false) {
        unload();
    }

    d_ptr->disconnect (this);    
    delete d_ptr;
}

void PluginLoader::setPluginPath (const QString & path) {
    d_ptr->m_pluginDir = path;
}

QString PluginLoader::pluginPath () const {
    return d_ptr->m_pluginDir.absolutePath();
}

int PluginLoader::pluginCount () const {
    return d_ptr->m_loaders.count();
}

int PluginLoader::methodCount () const {
    return d_ptr->m_loadedMethods.count();
}

QString PluginLoader::pluginName (int at) {

    if (at < 0 || at >= d_ptr->m_loaders.count()) {
        qWarning() << "No plugin available with index" << at;
        return "";
    }
    
    QString path = d_ptr->m_loaders.at (at)->fileName();
    return PluginLoaderPrivate::loaderToName (d_ptr->m_loaders.at(at));
}

ShareUI::MethodBase * PluginLoader::method (int at) {

    ShareUI::MethodBase * met = 0;

    if (d_ptr->m_loadedMethods.count() > at) {
        met = d_ptr->m_loadedMethods.at (at);
    }
    
    return met;
}

bool PluginLoader::loadPlugins () {

    if (d_ptr->m_loaders.count() > 0) {
        qWarning() << "First release old plugins";
        return false;
    }
    
    QDir dir = d_ptr->m_pluginDir;

    // Get list of plugin files
    QStringList filters;
    filters << SHARE_UI_PLUGIN_FILTER;
    dir.setNameFilters (filters);
    QStringList soFiles = dir.entryList (filters, QDir::Files, QDir::Name);
    
    // Load found plugins
    QStringList::const_iterator iter;
    for (iter = soFiles.constBegin(); iter != soFiles.constEnd(); ++iter) {
        
        qDebug() << "Loading plugin" << *iter;
        QPluginLoader * loader = new QPluginLoader (this);
        loader->setFileName (dir.filePath((*iter)));
        qDebug() << "Method loader hints set";
        loader->setLoadHints (QLibrary::ExportExternalSymbolsHint);
        
        // Check if plugin can be loaded
        if (loader->load() == true) {
            QObject * obj = loader->instance();
            
            ShareUI::PluginBase * plugin =
                qobject_cast<ShareUI::PluginBase *>(obj);
                
            if (plugin != 0 && plugin->init()) {
            
                d_ptr->m_pluginLoaderMap.insert (plugin, loader);
            
                QList <ShareUI::MethodBase *> pluginMethods =
                    plugin->methods (this);
                
                int loaded = 0;
                for (int i = 0; i < pluginMethods.size(); ++i) {
                    ShareUI::MethodBase * met = pluginMethods.at (i);
                                        
                    if (met != 0) {
                        d_ptr->m_methodPluginMap.insert (met, plugin);
                        d_ptr->m_loadedMethods.append (met);
                        ++loaded;
                        
                        connect (met, SIGNAL (visible(bool)), d_ptr,
                            SLOT (methodVisible(bool)));
                    }
                }
                
                qDebug() << "Plugin loaded with" << loaded << "new methods";
                d_ptr->m_loaders.append (loader);
                
                QObject::connect (plugin,
                    SIGNAL (newMethod(ShareUI::MethodBase*)), d_ptr,
                    SLOT (newMethodFromPlugin(ShareUI::MethodBase*)));
            
            } else {
                if (plugin != 0) {
                    qCritical() << "Initalization failed with plugin";
                    delete plugin;
                }
            
                qCritical() << "Plugin not accepted";
                delete loader;
            }
            
        } else {
            qCritical() << "Failed to load plugin:" << loader->errorString();
            delete loader;
        }

    }
    
    if (d_ptr->m_loaders.count() > 0) {
        qDebug() << "Load summary:";
        qDebug() << d_ptr->m_loaders.count() << "Share UI plugin(s) loaded with"
            << d_ptr->m_loadedMethods.count() << "method(s)";
        return true;
    } else {
        qWarning() << "No plugins found";
        return false;
    }
}

void PluginLoader::unload() {

    qDebug() << "Unloading all plugins";
    for (int i = 0; i < d_ptr->m_loadedMethods.count(); ++i) {
        delete d_ptr->m_loadedMethods.at (i);
    }
    d_ptr->m_loadedMethods.clear();

    for (int i = 0; i < d_ptr->m_loaders.count(); ++i) {
        QPluginLoader * loader = d_ptr->m_loaders.at(i);       
        qDebug() << loader->unload();
        delete loader;
    }
    d_ptr->m_loaders.clear();
}

QList <ShareUI::MethodBase *> PluginLoader::methods () const {
    return d_ptr->m_loadedMethods;
}

QString PluginLoader::pluginNameForMethod (ShareUI::MethodBase * method) const {

    return d_ptr->pluginNameForMethod (method);
}

bool PluginLoader::methodOrderingValues (ShareUI::MethodBase * method,
    int & order, int & suborder) {
    
    if (method == 0) {
        return false;
    }
    
    ShareUI::MethodBase::Type type = method->type();
    
    // Handle promoted methods (with check)
    bool isPromoted = (type == ShareUI::MethodBase::TYPE_PROMOTED);
    if (isPromoted == true) {
        int topOrder = d_ptr->promotedOrderValue (method);
        if (topOrder == -1) {
            isPromoted = false;
        } else {
            order = topOrder;
            suborder = method->order();            
        }
    }
    
    // Handle normal methods
    if (isPromoted == false) {
        if (type == ShareUI::MethodBase::TYPE_WEB_SERVICE) {
            order = -1;
            suborder = d_ptr->subOrderValue (method, type);
        
        // Default type is other    
        } else {
            if (type != ShareUI::MethodBase::TYPE_OTHER) {
                qWarning() << "Type of method" << method->id()
                    << "changed to other from" << type;
            }
        
            order = -2;
            suborder = d_ptr->subOrderValue (method, type);
        }
    }

    return true;
}

// - private ------------------------------------------------------------------

PluginLoaderPrivate::PluginLoaderPrivate (const QString & pluginDir,
    const QString & confFile, PluginLoader * parent) : QObject (parent),
    m_pluginDir (pluginDir), m_pluginConfig (confFile, QSettings::IniFormat) {
    
    m_promotedPlugins = m_pluginConfig.value (
        "promoted/plugins").toStringList();    
        
    buildRegExpList (m_pluginConfig.value ("services/order").toStringList(),
        m_serviceOrder);    
    
    buildRegExpList (m_pluginConfig.value ("others/order").toStringList(),
        m_otherOrder);

}

PluginLoaderPrivate::~PluginLoaderPrivate () {

}

void PluginLoaderPrivate::buildRegExpList (const QStringList & input,
    QList<QRegExp> & output) {
        
    QStringListIterator iter (input);
    while (iter.hasNext()) {
        output.append (QRegExp (iter.next()));
    }
}

int PluginLoaderPrivate::promotedOrderValue (ShareUI::MethodBase * method) {

    QString name = pluginNameForMethod (method);
    if (name.isEmpty() == true) {
        return -1;
    }
    
    int index = m_promotedPlugins.indexOf (name);
    
    if (index >= 0) {
        index = m_promotedPlugins.count() - index;
    }
    
    return index;
}

int PluginLoaderPrivate::subOrderValue (ShareUI::MethodBase * method,
    ShareUI::MethodBase::Type type) {
    
    // No id return always 0
    QString id = method->id();
    if (id.isEmpty() == true) {
        return 0;
    }    

    QList<QRegExp> & list = m_otherOrder;  
    if (type == ShareUI::MethodBase::TYPE_WEB_SERVICE) {
        list = m_serviceOrder;
    }
    
    // No list return always 0
    if (list.isEmpty() == true) {
        return 0;
    }    
        
    //Construct longId used in lists
    QString longId = pluginNameForMethod (method);
    longId.append ("/");
    longId.append (id);
    
    int found = findRegExp (list, longId);
    if (found >= 0) {
        return list.count() - found;
    }    

    return 0;
}

int PluginLoaderPrivate::findRegExp (const QList<QRegExp> & list,
    const QString & name) {
    
    int i = 0;
    
    QListIterator<QRegExp> iter (list);
    while (iter.hasNext()) {
        if (iter.next().exactMatch (name) == true) {
            return i;
        }
        ++i;
    }
    
    return -1;
}

QString PluginLoaderPrivate::loaderToName (QPluginLoader * loader) {

    static QString cleanPrefix = QLatin1String (SHARE_UI_PLUGIN_CLEAN_PREFIX);
    static QString cleanSuffix = QLatin1String (SHARE_UI_PLUGIN_CLEAN_SUFFIX);    

    QString path = loader->fileName();
    
    QFileInfo fInfo (path);
    
    QString name = fInfo.fileName();   
    
    // Cut out parts of filename
    if (name.startsWith (cleanPrefix) == true) {
        name.remove (0, cleanPrefix.length());
    }
    if (name.endsWith (cleanSuffix) == true) {
        name.chop (cleanSuffix.length());
    }

    return name;
}

void PluginLoaderPrivate::newMethodFromPlugin (ShareUI::MethodBase * method) {
    ShareUI::PluginBase * plugin = qobject_cast <ShareUI::PluginBase*>(
        sender());
    
    if (plugin != 0) {
        m_methodPluginMap.insert (method, plugin);
    }

    method->setParent (this);
    m_loadedMethods.append (method);
    
    connect (method, SIGNAL (visible(bool)), this, SLOT (methodVisible(bool)));
    
    Q_EMIT (newMethod(method));
}

QString PluginLoaderPrivate::pluginNameForMethod (
    ShareUI::MethodBase * method) const {
    
    QPluginLoader * loader = 0;
    ShareUI::PluginBase * plugin = 0;
    
    plugin = m_methodPluginMap.value (method);
    
    if (plugin != 0) {
        loader = m_pluginLoaderMap.value (plugin);
    } else {
        qCritical() << "Plugin for method" << method << "not found";
    }
    
    QString name;
    if (loader != 0) {
        name = loaderToName (loader);
    } else {
        qCritical() << "Loader for method" << method << "not found";
    }
    
    return name;
}

void PluginLoaderPrivate::methodVisible (bool visible) {
    ShareUI::MethodBase * method = qobject_cast<ShareUI::MethodBase*> (
        sender());
        
    if (method == 0) {
        qCritical() << "Invalid caller for" << __FUNCTION__;
        return;
    }
    
    Q_EMIT (methodVisible(method, visible));
}
