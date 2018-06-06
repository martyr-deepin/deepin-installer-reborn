/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include "service/screen_brightness.h"

#include <QDebug>
#include <QDir>
#include <QList>

#include "base/file_util.h"

namespace installer {

namespace {

class ScreenBrightness {
 public:
  ScreenBrightness(int actual, int max, const QString& dev_name) :
      actual_(actual), max_(max), dev_name_(dev_name) {}
  ~ScreenBrightness() {}

  void set(int value);
  int value() const {
    return static_cast<int>(actual_ * 100.0 / max_);
  }

 private:
  int actual_;
  int max_;
  QString dev_name_;
};

void ScreenBrightness::set(int value) {
  if (value < 0) {
    value = 0;
  } else if (value > 100) {
    value = 100;
  }

  const int brightness = static_cast<int>(value * max_ * 0.01);
  QString brightness_str = QString::number(brightness, 10);
  const QString brightness_file = QDir(dev_name_).filePath("brightness");
  WriteTextFile(brightness_file, brightness_str);
}

typedef QList<ScreenBrightness> ScreenBrightnessList;

ScreenBrightnessList GetScreenBrightnessList() {
  const char kSysBrightnessDir[] = "/sys/class/backlight";
  const char kMaxFile[] = "max_brightness";
  const char kActualFile[] = "actual_brightness";

  QDir sys(kSysBrightnessDir);
  ScreenBrightnessList result;
  for (const QFileInfo& device :
      sys.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    const QString dev_path = device.filePath();
    QDir dev(dev_path);
    const QString max_brightness = ReadFile(dev.filePath(kMaxFile));
    bool max_ok;
    const int max = max_brightness.toInt(&max_ok, 10);
    if (!max_ok) {
      qWarning() << "Failed to read max brightness:" << dev_path;
      continue;
    }

    const QString actual_brightness = ReadFile(dev.filePath(kActualFile));
    bool actual_ok;
    const int actual = actual_brightness.toInt(&actual_ok, 10);
    if (!actual_ok) {
      qWarning() << "Failed to read actual brightness:" << dev_path;
      continue;
    }

    ScreenBrightness screen(actual, max, dev_path);
    result.append(screen);
  }
  return result;
}

}  // namespace

void SetBrightness(int value) {
  for (ScreenBrightness& screen : GetScreenBrightnessList()) {
    screen.set(value);
  }
}

void IncreaseBrightness() {
  for (ScreenBrightness& screen : GetScreenBrightnessList()) {
    screen.set(screen.value() + 20);
  }
}

void DecreaseBrightness() {
  for (ScreenBrightness& screen : GetScreenBrightnessList()) {
    screen.set(screen.value() - 20);
  }
}

}  // namespace installer