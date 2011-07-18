
/*
 * share-ui -- Handset UX Share user interface
 * Copyright (c) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Jukka Tiihonen <jukka.t.tiihonen@nokia.com>
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

/**
 \mainpage Share UI Extension API Library
 \anchor main

 \section introduction Introduction
  Qt library used to extend Share UI with new sharing methods. Library defines
  all plugin interfaces and model classes used to maintain shared content inside
  Share UI.
  
  Eq. you can have plugin class MySharePlugin (that inherits
  ShareUI::PluginBase). When it's ShareUI::PluginBase::methods interface
  function is called it will generate one instance for each methods your plugin
  will offer (eg. MyEmailMethod and MySMSMethod). Then it's up to these method
  instances to tell how those are presented in Share UI. And what happens when
  user selects it as sharing end point.
  
  Class ShareUI::ItemContainer is the container for all items currently open in
  Share UI. ShareUI::Item is then related to one file (ShareUI::FileItem) or
  some another type item (eq. ShareUI::DataUriItem). Instances of these classes
  are given to Method plugins to upload those visibility (if content is
  suitable for method) and then to tell method what to share.

\section examples Examples
  See example page for simple example how to extend Share UI

*/
