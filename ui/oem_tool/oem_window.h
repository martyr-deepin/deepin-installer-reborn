// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_TOOL_OEM_WINDOW_H
#define INSTALLER_UI_OEM_TOOL_OEM_WINDOW_H

#include <QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QScrollArea>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;

namespace installer {

class OemWindow : public QFrame {
  Q_OBJECT

 public:
  explicit OemWindow(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  // Pages
  QCheckBox* skip_disk_space_insufficient_page_button_ = nullptr;
  QCheckBox* skip_virtual_machine_page_button_ = nullptr;
  QCheckBox* skip_select_language_page_button_ = nullptr;
  QCheckBox* skip_system_info_page_button_ = nullptr;
  QCheckBox* skip_partition_page_button_ = nullptr;

  // Language selection
  QCheckBox* use_default_locale_button_ = nullptr;
  QComboBox* default_locale_combo_ = nullptr;

  // System info
  QCheckBox* use_default_username_button_ = nullptr;
  QCheckBox* use_default_hostname_button_ = nullptr;
  QCheckBox* use_default_password_button_ = nullptr;
  QLineEdit* default_username_edit_ = nullptr;
  QLineEdit* default_hostname_edit_ = nullptr;
  QLineEdit* default_password_edit_ = nullptr;
  QLineEdit* vendor_name_edit_ = nullptr;
  QLineEdit* os_name_edit_ = nullptr;
  QLineEdit* os_version_edit_ = nullptr;

  // Partition
  QComboBox* default_fs_combo_ = nullptr;

  // Grub
  QLabel* grub_timeout_value_label_ = nullptr;
  QSlider* grub_timeout_slider_ = nullptr;
  QCheckBox* grub_disable_windows_button_ = nullptr;

  // Packages
  QLineEdit* uninstalled_packages_edit_ = nullptr;
  QLineEdit* hold_packages_edit_ = nullptr;

  // Dock
  QLineEdit* append_apps_to_dock_edit_ = nullptr;

  // Service
  QLineEdit* enabled_services_edit_ = nullptr;
  QLineEdit* disabled_services_edit_ = nullptr;

  QScrollArea* right_scroll_area_ = nullptr;

 private slots:
  void onGrubTimeoutSliderValueChanged(int value);
  void onGrubDisableWindowsButtonToggled(bool toggle);
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_TOOL_OEM_WINDOW_H