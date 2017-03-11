// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_UTILS_WIDGET_UTIL_H
#define INSTALLER_UI_UTILS_WIDGET_UTIL_H

#include <QWidget>
class QComboBox;
class QMenu;
class QLayout;

namespace installer {

// Append stylesheet to |widget| and re paint it.
void AppendStyleSheet(QWidget* widget, const QString& style);

// Delete layout items in |layout|.
void ClearLayout(QLayout* layout);

// Set background of child widget with name |child_name| in |root|
// to transparent. Returns false if failed.
bool SetChildTransparent(QWidget* root, const QString& child_name);

// Set |widget| to display fullscreen mode on primary screen.
// QWidget::showFullscreen() might fail to set fullscreen mode.
void ShowFullscreen(QWidget* widget);

// Resize |widget| to fill |geometry|.
void ShowFullscreen(QWidget* widget, const QRect& geometry);

// Set popup window background to transparent.
void SetQComboBoxTransparent(QComboBox* box);
// DEPRECATED. Not work.
void SetQMenuTransparent(QMenu* menu);

// Print type info about |root| and its children.
void WidgetTreeWalk(QWidget* root);

}  // namespace installer

#endif  // INSTALLER_UI_UTILS_WIDGET_UTIL_H
