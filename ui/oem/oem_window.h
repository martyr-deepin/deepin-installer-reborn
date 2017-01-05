// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_OEM_OEM_WINDOW_H
#define INSTALLER_UI_OEM_OEM_WINDOW_H

#include <QFrame>
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QScrollArea;
class QSlider;
class QStringListModel;

namespace installer {

class LanguageListModel;
class SettingsModel;

// Main window of deepin-installer-oem tool.
class OemWindow : public QFrame {
  Q_OBJECT

 public:
  explicit OemWindow(QWidget* parent = nullptr);

 private:
  void initConnections();
  void initUI();

  // Pages
  QCheckBox* disk_space_box_ = nullptr;
  QCheckBox* virtual_machine_box_ = nullptr;
  QCheckBox* skip_language_box_ = nullptr;
  QCheckBox* table_warning_box_ = nullptr;
  QCheckBox* system_info_box_ = nullptr;
  QCheckBox* partition_box_ = nullptr;

  // Language selection
  QComboBox* default_locale_combo_ = nullptr;
  LanguageListModel* language_model_ = nullptr;

  // System info
  QLineEdit* vendor_name_edit_ = nullptr;
  QCheckBox* lock_username_box_ = nullptr;
  QLineEdit* default_username_edit_ = nullptr;
  QCheckBox* lock_hostname_box_ = nullptr;
  QLineEdit* default_hostname_edit_ = nullptr;
  QCheckBox* lock_password_box_ = nullptr;
  QLineEdit* default_password_edit_ = nullptr;

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

  QListView* category_view_ = nullptr;
  QStringListModel* category_model_ = nullptr;
  QScrollArea* right_scroll_area_ = nullptr;

  SettingsModel* settings_model_ = nullptr;

 private slots:
  // Update tooltip of default_locale_combo_ when another language is selected.
  void onDefaultLocaleIndexChanged(int index);
};

}  // namespace installer

#endif  // INSTALLER_UI_OEM_OEM_WINDOW_H