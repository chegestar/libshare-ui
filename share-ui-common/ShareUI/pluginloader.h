 
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

#ifndef _PLUGIN_LOADER_H_
#define _PLUGIN_LOADER_H_

#include <QObject>
#include <QString>
#include <QPluginLoader>
#include <QDir>
#include <ShareUI/PluginInterface>
#include <ShareUI/MethodBase>
#include <ShareUI/PluginBase>
#include <QMap>
#include <QSettings>

namespace ShareUI {

    class PluginLoaderPrivate;

    /*!
       \class PluginLoader
       \brief Class to handle ShareUI plugin management (loading/unloading).

       \author Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
     */
    class PluginLoader : public QObject
    {
        Q_OBJECT
        public:
        
            /*!
              \brief Create new PluginLoader instance
              \param parent QObject parent
             */
            PluginLoader (QObject * parent = 0);
            
            ~PluginLoader();
            
            /*!
              \brief Change path where plugins are loaded. This need to be done
                     before loadPlugins is called.
              \param path Path where plugins are loaded
             */
            void setPluginPath (const QString & path);
                    
            /*!
              \brief Get current path used to load plugins
              \return Plugin path
             */
            QString pluginPath() const;
            
            /*!
              \brief Load all valid plugins found in plugin path. If plugins are
                     already loaded then this function will fail.
              \return true if one or more plugin was successfully loaded
             */
            bool loadPlugins ();
                    
            /*!
               \brief Unloads all plugins. Instances will be invalid after this.
             */
            void unload();
            
            /*!
              \brief Number of plugin loaded currently
              \return Number of plugins loaded
             */
            int pluginCount () const;
            
            /*!
              \brief Number or methods loaded currently
              \return Number of methods loaded
             */
            int methodCount () const;
            
            /*!
              \brief Get filename for plugin instance
              \param at Index of plugin
              \return Name of plugin or empty string if name not found
             */
            QString pluginName (int at);
            
            /*!
              \brief Get method with index
              \param at Index number of plugin. 0 .. (amount() - 1)
              \return Pointer to method class or null if at is out of range
             */
            ShareUI::MethodBase * method (int at);
            
            /*!
              \brief Deprecated. Use methods()
             */
            Q_DECL_DEPRECATED QList <ShareUI::MethodBase *> methodsWithType (
                ShareUI::MethodBase::Type type) const;
                
            /*!
              \brief Get current list of methods. Useful to call initial list of
                     loaded methods at the beginning. Later use newMethod signal
                     to do same calls to methods created later.
              \return List of methods
             */
            QList <ShareUI::MethodBase *> methods () const;
            
            /*!
              \brief Get plugin name for method
              \param method Method searched
              \return Plugin name for method
             */
            QString pluginNameForMethod (ShareUI::MethodBase * method) const;
            
            /*!
              \brief Get ordering values for method
              \param method Method which needs ordering values
              \param order Main ordering value (bigger is before)
              \param suborder Secondary ordering value (bigger is before)
              \return true if order values generated
             */
            bool methodOrderingValues (ShareUI::MethodBase * method, 
                int & order, int & suborder);
                       
        Q_SIGNALS:
            
            /*!
              \brief Signal emitted when method is activated after loading
              \param method Method activated
             */
            void newMethod (ShareUI::MethodBase * method);
            
            
            /*!
              \brief Signal to listen method visibility changes. Better to use this
                     than direct signal from ShareUI::MethodBase.
              \param method Method which visibility state has changed
              \param visible If true method should be presented to user. If false
                             then it must be hidden from the user.
             */
            void methodVisible (ShareUI::MethodBase * method, bool visible);
            
        private:
        
            Q_DISABLE_COPY(PluginLoader)
            PluginLoaderPrivate * const d_ptr; //!< Private data
    };

}

#endif /* #ifndef _PLUGIN_LOADER_H_ */