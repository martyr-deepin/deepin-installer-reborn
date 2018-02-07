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

#ifndef INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H
#define INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H

#include <QFrame>
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStringListModel;
class QTextEdit;

#include "oem/models/oem_settings_item.h"

namespace installer {

// Display oem settings item options.
class OemSettingsItemView : public QFrame {
  Q_OBJECT
 public:
  explicit OemSettingsItemView(QWidget* parent = nullptr);

  void updateItem(const OemSettingsItem& item);

 signals:
  // Emitted when value in |item| is changed.
  void itemChanged(const OemSettingsItem& item);

 private:
  void initConnections();
  void initUI();

  // Enable/disable custom value area.
  void enableCustomValue(bool enable);

  // Get value in custom area.
  QVariant getCustomValue();

  // Update text in value_ label.
  void updateCurrentValue();

  // Update value in custom area to current value of item.
  void updateCustomValue();

  QLabel* title_ = nullptr;
  QLabel* name_ = nullptr;
  QLabel* desc_ = nullptr;
  QLabel* type_ = nullptr;
  QLabel* default_value_ = nullptr;
  QLabel* value_ = nullptr;
  QPushButton* use_default_value_btn_ = nullptr;
  QPushButton* custom_bool_ = nullptr;
  QComboBox* custom_combo_ = nullptr;
  QLineEdit* custom_line_edit_ = nullptr;
  QSpinBox* custom_spin_box_ = nullptr;
  QTextEdit* custom_text_edit_ = nullptr;

  OemSettingsItem item_;
  QStringListModel* custom_list_model_ = nullptr;

 private slots:
  // Enable or disable custom value area when use_default_value_btn_ is toggled.
  void onUseDefaultValueButtonToggled(bool checked);

  // Handles changed events in custom area.
  void onCustomWidgetValueChanged();
};

}  // namespace installer

#endif  // INSTALLER_OEM_VIEWS_OEM_SETTINGS_ITEM_VIEW_H
