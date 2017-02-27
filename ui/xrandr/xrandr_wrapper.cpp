// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/xrandr/xrandr_wrapper.h"

#include <QRegularExpression>

#include "base/command.h"
#include "base/string_util.h"

namespace installer {
namespace {

const char kXRandRApp[] = "xrandr";

// Parse "Screen 0: minimum 320 x 200, current 1920 x 1080, maximum 8192 x 8192"
bool ParseScreen(const QString& text, Screen& screen) {
  QRegularExpression reg("Screen (\\d+): minimum (\\d+) x (\\d+), "
                         "current (\\d+) x (\\d+), maximum (\\d+) x (\\d+)");

  QRegularExpressionMatch match = reg.match(text);
  if (match.hasMatch() && (match.lastCapturedIndex() == 7)) {
    screen.id = match.captured(1).toInt();
    screen.min_width = match.captured(2).toInt();
    screen.min_height = match.captured(3).toInt();
    screen.curr_width = match.captured(4).toInt();
    screen.curr_height = match.captured(5).toInt();
    screen.max_width = match.captured(6).toInt();
    screen.max_height = match.captured(7).toInt();
    return true;
  } else {
    return false;
  }
}

// Parses "1920x1080     60.00 +  59.93*   40.00"
bool ParseMode(const QString& text, Mode& mode) {
  if (text.contains("+")) {
    mode.is_preferred = true;
  }
  if (text.contains("*")) {
    mode.is_used = true;
  }
  const QStringList items = text.trimmed().split(' ');
  if (items.length() > 1) {
    mode.name = items[0];
    const QStringList values = items[0].split('x');
    if (values.length() == 2) {
      mode.width = values[0].toInt();
      mode.height = values[1].toInt();
      return true;
    }
  }

  return false;
}

// Parses "eDP-1 connected primary 1920x1080+0+0"
bool ParseOutput(const QString& text, Output& output) {
  bool ok = false;
  const QStringList& items = text.split(' ');
  if (items.length() > 2) {
    output.name = items[0];
    output.is_connected = (items[1] == "connected");
    QString geom;
    if (items[2] == "primary") {
      geom = items[3];
      output.is_primary = true;
    } else {
      geom = items[2];
    }
    if (output.is_connected) {
      // Matches "1920x1080+0+0"
      QRegularExpression reg("(\\d+)x(\\d+)[^\\d](\\d+)[^\\d](\\d+)");
      QRegularExpressionMatch match = reg.match(geom);

      if (match.hasMatch() && (match.lastCapturedIndex() == 4)) {
        output.width = match.captured(1).toInt();
        output.height = match.captured(2).toInt();
        output.x = match.captured(3).toInt();
        output.y = match.captured(4).toInt();
        ok = true;
      }
    } else {
      ok = true;
    }
  }
  return ok;
}

}  // namespace

QDebug& operator<<(QDebug& debug, const Screen& screen) {
  debug << "Screen {"
        << screen.id
        << screen.min_width << "x" << screen.min_height
        << screen.curr_width << "x" << screen.curr_height
        << screen.max_width << "x" << screen.max_height
        << "}";
  return debug;
}

QDebug& operator<<(QDebug& debug, const Mode& mode) {
  debug << "Mode {"
        << mode.name
        << mode.width << "x" << mode.height
        << (mode.is_used ? "used" : " ")
        << (mode.is_preferred ? "preferred" : " ")
        << "}";
  return debug;
}

QDebug& operator<<(QDebug& debug, const Output& output) {
  debug << "Output {"
        << output.name
        << (output.is_connected ? "connected" : "disconnected")
        << (output.is_primary ? "primary" : " ")
        << output.width << "x" << output.height
        << output.x << "x" << output.y
        << "\n";
  for (const Mode& mode : output.modes) {
    debug << "  " << mode << "\n";
  }
  debug << "}";
  return debug;
}

QDebug& operator<<(QDebug& debug, const XRandR& xrandr) {
  for (const Screen& screen : xrandr.screens) {
    debug << screen << "\n";
  }
  for (const Output& output : xrandr.outputs) {
    debug << output << "\n";
  }
  return debug;
}

bool RunXRandR(QString& output) {
  QString err;
  if (!installer::SpawnCmd(kXRandRApp, {}, output, err)) {
    qCritical() << "Run xrandr failed:" << output << err;
    return false;
  } else {
    return true;
  }
}

bool ParseXRandR(const QString& text, XRandR& xrandr) {
  for (const QString& line : text.split('\n')) {
    if (line.isEmpty()) {
      // Do nothing.
    } else if (line.startsWith("Screen")) {
      Screen screen;
      if (ParseScreen(line, screen)) {
        xrandr.screens.append(screen);
      } else {
        qCritical() << "Parse screen failed:" << line;
      }
    } else if (line.startsWith("  ")) {
      Mode mode;
      if (!xrandr.outputs.isEmpty()) {
        Output& output = xrandr.outputs.last();
        if (ParseMode(line, mode)) {
          output.modes.append(mode);
        } else {
          qCritical() << "Parse mode failed:" << line;
        }
      } else {
        qWarning() << "xrandr output list is empty";
      }
    } else {
      Output output;
      if (ParseOutput(line, output)) {
        xrandr.outputs.append(output);
      } else {
        qCritical() << "Parse output failed:" << line;
      }
    }
  }
  return true;
}

bool SwitchMode(const XRandR& xrandr) {
  QList<Output> connected_outputs;
  for (const Output& output : xrandr.outputs) {
    if (output.is_connected && (!output.modes.isEmpty())) {
      connected_outputs.append(output);
    }
  }

  if (connected_outputs.length() == 0) {
    qCritical() << "No connected output found!";
    return false;
  }

  QStringList args;
  if (connected_outputs.length() == 1) {
    // Only one monitor is connected, using `prefer` mode
    const Output& output = connected_outputs.first();
    args << "--output" << output.name << "--preferred" << "--scale" << "1x1";
  } else {
    // Check mirror mode
    int starts_from_o_point = 0;
    for (const Output& output : connected_outputs) {
      if (output.x == 0 && output.y == 0) {
        starts_from_o_point ++;
      }
    }

    // if (starts_from_o_point == connected_outputs.length()) {
    if (starts_from_o_point > 1) {
      // If it was mirror mode, set the first output as primary
      for (int i = 0; i < connected_outputs.length(); i++) {
        const Output& output = connected_outputs.at(i);
        args << "--output" << output.name << "--preferred"
             << "--scale" << "1x1";
        if (i == 0) {
          args << "--primary";
        } else {
          // Expand horizontally
          args << "--right-of" << connected_outputs.at(i-1).name;
        }
      }
    } else {
      int primary_index = 0;
      for (int i = 0; i < connected_outputs.length(); i++) {
        if (connected_outputs.at(i).is_primary) {
          primary_index = i;
          break;
        }
      }
      if (primary_index < connected_outputs.length() - 1) {
        // Switch primary output
        for (int i = 0; i < connected_outputs.length(); i++) {
          const Output& output = connected_outputs.at(i);
          if (i == 0) {
            args << "--output" << output.name << "--scale" << "1x1"
                 << "--preferred";
          } else {
            args << "--output" << output.name << "--scale" << "1x1"
                 << "--preferred" << "--right-of"
                 << connected_outputs.at(i-1).name;
          }
          if (i == primary_index + 1) {
            // Set next output as primary
            args << "--primary";
          }
        }
      } else {
        // If last connected output is primary, use mirror mode.
        // Get minimum mode.
        Mode mini_mode = connected_outputs.at(0).modes.at(0);
        for (const Output& output : connected_outputs) {
          Mode pref_mode = output.modes.at(0);
          for (const Mode& mode : output.modes) {
            if (mode.is_preferred) {
              pref_mode = mode;
            }
          }

          if (mini_mode.width > pref_mode.width &&
              mini_mode.height > pref_mode.height) {
            mini_mode = pref_mode;
          }
        }

        for (const Output& output : connected_outputs) {
          args << "--output" << output.name << "--scale" << "1x1"
               << "--mode" << mini_mode.name << "--pos" << "0x0";
        }
      }
    }
  }

  QString out, err;
  qDebug() << "xrandr args:" << args;
  if (installer::SpawnCmd(kXRandRApp, args, out, err)) {
    return true;
  } else {
    qCritical() << "Run xrandr failed:" << err << ", args:" << args;
    return false;
  }
}

bool SwitchModeWrapper() {
  QString out;
  if (!installer::RunXRandR(out)) {
    qCritical() << "Run xrandr failed";
    return false;
  }
  if (out.isEmpty()) {
    qCritical() << "xrandr returns an empty string!";
    return false;
  }

  installer::XRandR xrandr;
  if (!installer::ParseXRandR(out, xrandr)) {
    qCritical() << "Parse XRandR failed!";
    return false;
  }

  if (!installer::SwitchMode(xrandr)) {
    qCritical() << "Switch xrandr mode failed!";
    return false;
  }

  return true;
}

bool SwitchToMirrorMode() {
  QString out;
  if (!installer::RunXRandR(out)) {
    qCritical() << "Run xrandr failed";
    return false;
  }
  if (out.isEmpty()) {
    qCritical() << "xrandr returns an empty string!";
    return false;
  }

  installer::XRandR xrandr;
  if (!installer::ParseXRandR(out, xrandr)) {
    qCritical() << "Parse XRandR failed!";
    return false;
  }

  QStringList args;
  QList<Output> connected_outputs;
  for (const Output& output : xrandr.outputs) {
    if (output.is_connected && (!output.modes.isEmpty())) {
      connected_outputs.append(output);
    }
  }

  if (connected_outputs.length() == 0) {
    qCritical() << "No connected output found!";
    return false;
  }

  if (connected_outputs.length() == 1) {
    qWarning() << "Only one monitor is connected, no need to set mirror mode.";
    return true;
  }

  // If last connected output is primary, use mirror mode.
  // Get minimum mode.
  Mode mini_mode = connected_outputs.at(0).modes.at(0);
  for (const Output& output : connected_outputs) {
    Mode pref_mode = output.modes.at(0);
    for (const Mode& mode : output.modes) {
      if (mode.is_preferred) {
        pref_mode = mode;
      }
    }

    if (mini_mode.width > pref_mode.width &&
        mini_mode.height > pref_mode.height) {
      mini_mode = pref_mode;
    }
  }

  for (const Output& output : connected_outputs) {
    args.append({"--output", output.name, "--scale", "1x1",
                 "--mode", mini_mode.name, "--pos", "0x0"});
  }

  QString err;
  qDebug() << "xarndr args:" << args;
  if (installer::SpawnCmd(kXRandRApp, args, out, err)) {
    return true;
  } else {
    qCritical() << "Run xrandr failed:" << err << ", args:" << args;
    return false;
  }
}

}  // namespace installer
