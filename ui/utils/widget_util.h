// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_UTILS_WIDGET_UTIL_H
#define INSTALLER_UI_UTILS_WIDGET_UTIL_H

#include <QWidget>
class QComboBox;
class QMenu;

namespace installer {

// Set background of child widget with name |child_name| in |root|
// to transparent. Returns false if failed.
bool SetChildTransparent(QWidget* root, const QString& child_name);

// Set popup window background to transparent.
void SetQComboBoxTransparent(QComboBox* box);
// DEPRECATED. Not work.
void SetQMenuTransparent(QMenu* menu);

// Print type info about |root| and its children.
void WidgetTreeWalk(QWidget* root);

}  // namespace installer

#endif  // INSTALLER_UI_UTILS_WIDGET_UTIL_H
