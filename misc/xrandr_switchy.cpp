// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include <QDebug>
#include <QList>

#include "base/command.h"

namespace {

const char kXRandRApp[] = "xrandr";

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

// `xrandr` result.
struct XRandR {
 public:
  XRandR() : screens(), outputs() {}
  ScreenList screens;
  OutputList outputs;
};

Screen ParseScreen(const QString& text) {
  Q_UNUSED(text);
  Screen screen;
  return screen;
}

XRandR ParseXRandR(const QString& text) {
  Q_UNUSED(text);
  XRandR xrandr;
  for (const QString& line : text.split('\n')) {
    if (line.startsWith("Screen")) {
      Screen screen = ParseScreen(line);
      xrandr.screens.append(screen);
    }
  }
  return xrandr;
}

// Switch to next mode
void SwitchMode(const XRandR& xrandr) {
  Q_UNUSED(xrandr);
}

}

int main(void) {
  QString out, err;
  if (!installer::SpawnCmd(kXRandRApp, {}, out, err)) {
    qCritical() << "Run xrandr failed:" << out << err;
    return 1;
  }

  if (out.isEmpty()) {
    qCritical() << "xrandr returns an empty string!";
    return 1;
  }

  XRandR xrandr = ParseXRandR(out);

  SwitchMode(xrandr);

  return 0;
}