/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INSTALLER_UI_UTILS_WIDGET_UTIL_H
#define INSTALLER_UI_UTILS_WIDGET_UTIL_H

#include <QWidget>
#include <functional>

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

const QPixmap renderPixmap(const QString &path);

template <typename T>
void addTransLate(T &t, std::function<void (QString)> function, const QString &tr) {
    t.push_back(std::pair<std::function<void (QString)>, QString>(function, tr));
}
}  // namespace installer

#endif  // INSTALLER_UI_UTILS_WIDGET_UTIL_H
