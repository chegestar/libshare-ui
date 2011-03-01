
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

#ifndef _SHARE_WIDGETS_UI_LOADER_H_
#define _SHARE_WIDGETS_UI_LOADER_H_

#include <QObject>
#include <ShareWidgets/ApplicationViewInterface>

namespace ShareWidgets {

    class UiLoaderPrivate;

    /*!
      \class UiLoader
      \brief Plugin loader for Share UI implementations
     */
    class UiLoader : public QObject {
    Q_OBJECT
    public:
        /*!
          \brief Consturctor
          \param parent QObject parent
         */
        UiLoader (QObject * parent = 0);
        virtual ~UiLoader ();

        /*!
          \brief Set the path for the plugin loader and load the plugin
          \param pluginLoader Pluginloader with loaded methods
          \return <code>true</code> if plugin loaded successfully, else
                  <code>false</code>
         */
        bool loadPlugin (ShareUI::PluginLoader * pluginLoader);

        /*!
          \brief Get the QApplication pointer from the plugin
          \param argc Count of arguments passed
          \param argv List of arguments passed
          \return QApplication pointer
         */
        QApplication * getApplicationPointer (int argc, char **argv);
        
        /*!
          \brief Show the sharing UI
          \param container Container with content to be shared
          \return <code>true</code> if the UI presentation succeeded, else
                  <code>false</code>
         */
        bool showUI (ShareUI::ItemContainer * container);
                        
    Q_SIGNALS:

        /*!
          \brief Signal emitted to say that the method plugin loading can start
         */
        void startLoadingPlugins ();

    private:
        Q_DISABLE_COPY (UiLoader)
        UiLoaderPrivate * const d_ptr; //!< Private data
    };
}

#endif
