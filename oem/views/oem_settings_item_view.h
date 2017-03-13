// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H
#define INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H

#include <QFrame>
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;

#include "oem/models/oem_settings_item.h"

namespace installer {

// Display oem settings item options.
class OemSettingsItemView : public QFrame {
  Q_OBJECT
 public:
  explicit OemSettingsItemView(QWidget* parent = nullptr);

  void updateItem(const OemSettingsItem& item);

 private:
  void initConnections();
  void initUI();

  // Enable/disable custom value area.
  void enableCustomValue(bool enable);

  QLabel* title_ = nullptr;
  QLabel* name_ = nullptr;
  QLabel* desc_ = nullptr;
  QLabel* type_ = nullptr;
  QLabel* default_value_ = nullptr;
  QLabel* value_ = nullptr;
  QPushButton* use_default_value_btn_ = nullptr;
  QPushButton* custom_bool_ = nullptr;
  QLineEdit* custom_line_edit_ = nullptr;
  QTextEdit* custom_text_edit_ = nullptr;

  OemSettingsItem item_;

 private slots:
  // Enable or disable custom value area when use_default_value_btn_ is toggled.
  void onUseDefaultValueButtonToggled(bool checked);
};

}  // namespace installer

#endif  // INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H
