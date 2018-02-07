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

#ifndef INSTALLER_UI_UTILS_XUTIL_H
#define INSTALLER_UI_UTILS_XUTIL_H

#include <X11/cursorfont.h>
#include <QString>
class QWidget;

namespace installer {
namespace utils {

// Get pointer mask (modifier type).
// Call QCursor::pos() to get pointer position.
Qt::MouseButton GetPointerMask();

// Let |widget| grab input focus.
void GrabFocus(QWidget* widget);

void GrabPointer(QWidget* widget);

// Move window to cursor position.
void MoveWindow(QWidget* widget);

// Move or resize window, based on mouse event.
// |border_width| is treated as border width. Cursor shapes changes in
// border area.
void MoveResizeWindow(QWidget* widget, int x, int y, int border_width = 6);

void ResizeWindow(QWidget* widget, const QString& corner);

// Check whether current cursor is at corner or edge of |widget|.
bool IsCornerEdget(QWidget* widget, int x, int y, int border_width);

// Undoes the effect of a previous XDefineCursor for this window.
void ResetCursorShape(QWidget* widget);

// Set specific cursor shape when cursor moves into |widget|.
// Cursor type is defined in /usr/include/X11/cursorfont.h
bool SetCursorShape(QWidget* widget, int cursor_id);

enum class CornerEdge {
  kInvalid = 0,
  kTop = 1,
  kRight = 2,
  kBottom = 4,
  kLeft = 8,
  kTopLeft = 1 | 8,
  kTopRight = 1 | 2,
  kBottomLeft = 4 | 8,
  kBottomRight = 4 | 2,
};

enum class X11WindowType {
  Desktop,
  Dialog,
  Dock,
  Menu,
  Normal,
  Splash,
  Toolbar,
  Utility,
};

CornerEdge GetCornerEdge(QWidget* widget, int x, int y, int border_width);

// Change type hint of a X11 window.
void SetWindowTypeHint(QWidget* widget, X11WindowType window_type);

// Enter/exit fullscreen mode.
void ShowFullscreenWindow(QWidget* widget, bool is_fullscreen);

// Shows the widget maximized.
// Note that |widget| shall be a window.
void ShowMaximizedWindow(QWidget* widget);

// Enter/exit minimized mode.
void ShowMinimizedWindow(QWidget* widget, bool minimized);

// Restores the widget after it has been maximized or minimized.
void ShowNormalWindow(QWidget* widget);

// Toggle window state between normal size and maximized size.
void ToggleMaximizedWindow(QWidget* widget);

// Update cursor shape when cursor reaches cursor or edge of widget border.
// Returns false if failed to change cursor shape.
bool UpdateCursorShape(QWidget* widget, int x, int y, int border_width = 6);

void SkipTaskbarPager(QWidget* widget);

void SetStayOnTop(QWidget* widget, bool);

void SetMouseTransparent(QWidget*, bool);

// Simulate a mouse click event.
void SimulateButtonEvent(Qt::MouseButton button, bool is_press);

// Simulate a mouse move event.
void SimulateMotionEvent(int x, int y);

}  // namespace utils
}  // namespace installer

#endif  // INSTALLER_UI_UTILS_XUTIL_H
