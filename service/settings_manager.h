// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
#define DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H

#include <QDir>
#include <QObject>

namespace service {

class SettingsManager : public QObject {
  Q_OBJECT

 public:
  SettingsManager();

  QString getLanguageListFile();

  // Get image path of window background.
  // First try to find in root of iso.
  // Then read from /usr/share/backgrounds/.
  // If not appropriate image is found, use the fallback image.
  QString getWindowBackground();

 private:
  QDir oem_dir_;
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SETTINGS_MANAGER_H
