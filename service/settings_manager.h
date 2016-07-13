// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H

#include <QDir>
#include <QObject>
class QSettings;

namespace service {

class SettingsManager : public QObject {
  Q_OBJECT

 public:
  SettingsManager();

  // Get boolean option value from settings file.
  // If |key| does not exist, returns false.
  bool getSettingsBool(const QString& key);

  // Get string option value from settings file.
  // If |key| does not exist, returns an empty string.
  QString getSettingsString(const QString& key);

  // Get variant option value from settings file.
  QVariant getSettingsValue(const QString& key);

  // Get image path of window background.
  // First try to find in root of iso.
  // Then read from /usr/share/backgrounds/.
  // If not appropriate image is found, use the fallback image.
  QString getWindowBackground();

 private:
  QDir oem_dir_;
  QSettings* oem_settings_ = nullptr;
  QSettings* default_settings_ = nullptr;
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
