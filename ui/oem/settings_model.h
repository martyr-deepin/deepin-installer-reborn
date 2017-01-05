// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_SETTINGS_MODEL_H
#define INSTALLER_UI_OEM_SETTINGS_MODEL_H

#include <QObject>

namespace installer {

// Handles read/write of settings file.
// All settings are saved in $HOOME/oem/ folder.
class SettingsModel : public QObject {
  Q_OBJECT

 public:
  explicit SettingsModel(QObject* parent = nullptr);

  // Initialize oem folder. Returns false if failed.
  // Call this method before accessing any other methods of this class.
  bool initOemFolder();

  // Pages
  bool skipDiskSpacePage() const;
  bool skipVirtualMachinePage() const;
  bool skipLanguagePage() const;
  bool skipTableWarningPage() const;
  bool skipSystemInfoPage() const;
  bool skipPartitionPage() const;

  // Language selection
  bool useDefaultLocale() const;

 public slots:
  // Pages
  void setSkipDiskSpacePage(bool checked);
  void setSkipVirtualMachinePage(bool checked);
  void setSkipLanguagePage(bool checked);
  void setSkipTableWarningPage(bool checked);
  void setSkipSystemInfoPage(bool checked);
  void setSkipPartitionPage(bool checked);

  // Language selection
  void setUseDefaultLocale(bool checked);
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_SETTINGS_MODEL_H
