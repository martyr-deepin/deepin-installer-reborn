// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
#define INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H

#include <QDebug>
#include <QList>

namespace installer {

struct Screen {
 public:
  Screen() : id(0), min_width(0), min_height(0), curr_width(0), curr_height(0),
             max_width(0), max_height(0) {}

  int id;
  int min_width;
  int min_height;
  int curr_width;
  int curr_height;
  int max_width;
  int max_height;
};
typedef QList<Screen> ScreenList;
QDebug& operator<<(QDebug& debug, const Screen& screen);

// Output mode.
struct Mode {
 public:
  Mode() : name(), width(0), height(0), is_used(false), is_preferred(false) {}

  QString name;  // mode name
  int width;
  int height;
  bool is_used;  // currently used
  bool is_preferred;  // Preferred mode
};
typedef QList<Mode> ModeList;
QDebug& operator<<(QDebug& debug, const Mode& mode);

struct Output {
 public:
  Output() : name(), is_connected(false), is_primary(false),
             width(0), height(0), x(0), y(0), modes() {}

  QString name;  // Output name, like "eDP-1", "HDMI-1"
  bool is_connected;
  bool is_primary;
  int width;
  int height;
  int x;
  int y;
  ModeList modes;
};
typedef QList<Output> OutputList;
QDebug& operator<<(QDebug& debug, const Output& output);

// `xrandr` result.
struct XRandR {
 public:
  XRandR() : screens(), outputs() {}

  ScreenList screens;
  OutputList outputs;
};
QDebug& operator<<(QDebug& debug, const XRandR& xrandr);

// Run `xrandr` and get its output.
bool RunXRandR(QString& output);

// Parse output of `xrandr`.
bool ParseXRandR(const QString& text, XRandR& xrandr);

// Switch between display modes.
// If only one output is found, use preferred mode;
// Else
//   If last connected output is primary, use mirror mode;
//   Else set next connected output as primary
//   End if.
// End if.
bool SwitchMode(const XRandR& xrandr);

// Parse xrandr output and switch mode
bool SwitchModeWrapper();

// Switch to mirror mode if multiple monitors are connected.
bool SwitchToMirrorMode();

}  // namespace installer

#endif  // INSTALLER_UI_XRANDR_XRANDR_WRAPPER_H
