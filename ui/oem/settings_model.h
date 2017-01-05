// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_SETTINGS_MODEL_H
#define INSTALLER_UI_OEM_SETTINGS_MODEL_H

#include <QObject>
class QCheckBox;
class QLineEdit;

namespace installer {

// Handles read/write of settings file.
// All settings are saved in $HOOME/oem/ folder.
// TODO(xushaohua): Add process lock.
class SettingsModel : public QObject {
  Q_OBJECT

 public:
  explicit SettingsModel(QObject* parent = nullptr);

  // Initialize oem folder. Returns false if failed.
  // Call this method before accessing any other methods of this class.
  bool initOemFolder();

  // Create a QCheckBox widget, binding to |property|.
  virtual QCheckBox* addCheckBox(const QString& property);

  // Create a QLineEdit widget, binding to |property|.
  virtual QLineEdit* addLineEdit(const QString& property);

  // Language selection
  QString defaultLocale() const;

 public slots:
   // Language selection
  void setDefaultLocale(const QString& locale);
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_SETTINGS_MODEL_H
