// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/oem/oem_window.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QSplitter>
#include <QStringListModel>
#include <QVBoxLayout>

#include "ui/models/language_list_model.h"
#include "ui/oem/settings_model.h"

namespace installer {
namespace {
// Add 20px between sections
const int kSectionSpace = 20;

}  // namespace

OemWindow::OemWindow(QWidget* parent) : QFrame(parent) {
  this->setObjectName("oem_window");

  settings_model_ = new SettingsModel(this);

  if (settings_model_->initOemFolder()) {
    this->initUI();
    this->initConnections();
  } else {
    // Display an error message if failed to init oem folder.
    QLabel* error_label = new QLabel(this);
    error_label->setObjectName("error_label");
    error_label->setText(tr("Failed to create $HOME/oem folder!"));
  }
}

void OemWindow::initConnections() {
  // Pages
  connect(disk_space_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipDiskSpacePage);
  connect(virtual_machine_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipVirtualMachinePage);
  connect(skip_language_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipLanguagePage);
  connect(table_warning_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipTableWarningPage);
  connect(system_info_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipSystemInfoPage);
  connect(partition_box_, &QCheckBox::toggled,
          settings_model_, &SettingsModel::setSkipPartitionPage);

  // Language selection
  connect(default_locale_combo_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &OemWindow::onDefaultLocaleIndexChanged);
}

void OemWindow::initUI() {
  // Pages
  disk_space_box_ = new QCheckBox(this);
  disk_space_box_->setObjectName("disk_space_box");
  disk_space_box_->setText(tr("Skip disk space insufficient page"));
  disk_space_box_->setChecked(settings_model_->skipDiskSpacePage());

  virtual_machine_box_ = new QCheckBox(this);
  virtual_machine_box_->setObjectName("virtual_machine_box");
  virtual_machine_box_->setText(tr("Skip virtual machine warning page"));
  virtual_machine_box_->setChecked(settings_model_->skipVirtualMachinePage());

  skip_language_box_ = new QCheckBox(this);
  skip_language_box_->setObjectName("skip_language_box");
  skip_language_box_->setText(tr("Skip language selection page"));
  skip_language_box_->setChecked(settings_model_->skipLanguagePage());

  table_warning_box_ = new QCheckBox(this);
  table_warning_box_->setObjectName("table_warning_box");
  table_warning_box_->setText(tr("Skip partition table warning page"));
  table_warning_box_->setChecked(settings_model_->skipTableWarningPage());

  system_info_box_ = new QCheckBox(this);
  system_info_box_->setObjectName("system_info_box");
  system_info_box_->setText(tr("Skip system info page"));
  system_info_box_->setChecked(settings_model_->skipSystemInfoPage());

  partition_box_ = new QCheckBox(this);
  partition_box_->setObjectName("partition_box");
  partition_box_->setText(tr("Skip partition page"));
  partition_box_->setChecked(settings_model_->skipPartitionPage());

  // Language selection
  QLabel* default_locale_label = new QLabel(this);
  default_locale_label->setObjectName("default_locale_label");
  default_locale_label->setText(tr("Default locale:"));

  default_locale_combo_ = new QComboBox(this);
  default_locale_combo_->setObjectName("default_locale_combo");
  language_model_ = new LanguageListModel(this);
  default_locale_combo_->setModel(language_model_);
  // Select default locale.
  const QString default_locale = settings_model_->defaultLocale();
  const QModelIndex locale_index = language_model_->localeIndex(default_locale);
  if (locale_index.isValid()) {
    default_locale_combo_->setCurrentIndex(locale_index.row());
  }

  QHBoxLayout* default_locale_layout = new QHBoxLayout();
  default_locale_layout->addWidget(default_locale_label);
  default_locale_layout->addWidget(default_locale_combo_);

  // System info
  vendor_name_edit_ = new QLineEdit();
  vendor_name_edit_->setPlaceholderText("vendor name");
  os_name_edit_ = new QLineEdit();
  os_name_edit_->setPlaceholderText("OS name");
  os_version_edit_ = new QLineEdit();
  os_version_edit_->setPlaceholderText("OS version");
  use_default_username_button_ = new QCheckBox("Use default username");
  use_default_hostname_button_ = new QCheckBox("Use default hostname");
  use_default_password_button_ = new QCheckBox("Use default password");
  default_username_edit_ = new QLineEdit();
  default_username_edit_->setPlaceholderText("default username");
  default_hostname_edit_ = new QLineEdit();
  default_hostname_edit_->setPlaceholderText("default hostname");
  default_password_edit_ = new QLineEdit();
  default_password_edit_->setPlaceholderText("default password");

  // Partition
  default_fs_combo_ = new QComboBox();
  default_fs_combo_->addItems({"btrfs", "ext4", "ext3", "xfs"});

  // Grub tab
  QLabel* grub_timeout_label = new QLabel();
  grub_timeout_label->setText("Grub menu timeout:");
  grub_timeout_slider_ = new QSlider();
//  grub_timeout_slider_->setMinimum(kGrubTimeoutMinimum);
//  grub_timeout_slider_->setMaximum(kGrubTimeoutMaximum);
  grub_timeout_slider_->setOrientation(Qt::Horizontal);
  grub_timeout_value_label_ = new QLabel();
  QHBoxLayout* grub_timeout_layout = new QHBoxLayout();
  grub_timeout_layout->addWidget(grub_timeout_label);
  grub_timeout_layout->addWidget(grub_timeout_slider_);
  grub_timeout_layout->addWidget(grub_timeout_value_label_);
  grub_disable_windows_button_ = new QCheckBox("Disable Windows in grub menu");
  grub_disable_windows_button_->setCheckable(true);

  // Packages
  uninstalled_packages_edit_ = new QLineEdit();
  uninstalled_packages_edit_->setPlaceholderText("packages to be uninstalled");
  hold_packages_edit_ = new QLineEdit();
  hold_packages_edit_->setPlaceholderText(
      "packages to be prevented from uninstalling");

  // Dock
  append_apps_to_dock_edit_ = new QLineEdit();
  append_apps_to_dock_edit_->setPlaceholderText("apps appending to dock");

  // Services
  enabled_services_edit_ = new QLineEdit();
  enabled_services_edit_->setPlaceholderText("enabled services");
  disabled_services_edit_ = new QLineEdit();
  disabled_services_edit_->setPlaceholderText("disabled services");

  QVBoxLayout* right_layout = new QVBoxLayout();
  right_layout->addWidget(disk_space_box_);
  right_layout->addWidget(virtual_machine_box_);
  right_layout->addWidget(skip_language_box_);
  right_layout->addWidget(system_info_box_);
  right_layout->addWidget(partition_box_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addLayout(default_locale_layout);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(vendor_name_edit_);
  right_layout->addWidget(os_name_edit_);
  right_layout->addWidget(os_version_edit_);
  right_layout->addWidget(use_default_username_button_);
  right_layout->addWidget(default_username_edit_);
  right_layout->addWidget(use_default_hostname_button_);
  right_layout->addWidget(default_hostname_edit_);
  right_layout->addWidget(use_default_password_button_);
  right_layout->addWidget(default_password_edit_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(default_fs_combo_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addLayout(grub_timeout_layout);
  right_layout->addWidget(grub_disable_windows_button_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(uninstalled_packages_edit_);
  right_layout->addWidget(hold_packages_edit_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(append_apps_to_dock_edit_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(enabled_services_edit_);
  right_layout->addWidget(disabled_services_edit_);

  QFrame* right_frame = new QFrame();
  right_frame->setObjectName("right_frame");
  right_frame->setContentsMargins(0, 0, 0, 0);
  right_frame->setLayout(right_layout);

  right_scroll_area_ = new QScrollArea();
  right_scroll_area_->setWidget(right_frame);

  category_view_ = new QListView(this);
  category_model_ = new QStringListModel(this);
  category_view_->setModel(category_model_);
  const QStringList categories = {
      "Pages",
      "Languages",
      "System Info",
      "Partition",
      "Installation",
      "Grub",
      "Desktop",
      "Packages",
      "Services",
      "Miscellaneous",
  };
  category_model_->setStringList(categories);
  category_model_->setStringList(categories);

  QSplitter* splitter = new QSplitter(this);
  splitter->setContentsMargins(0, 0, 0, 0);
  splitter->addWidget(category_view_);
  splitter->addWidget(right_scroll_area_);

  // TODO(xushaohua): Add buttons
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(splitter);

  this->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);
}

void OemWindow::onDefaultLocaleIndexChanged(int index) {
  const LanguageItem lang = language_model_->languageItemAt(index);
  // Update tooltip of default_locale_combo_.
  default_locale_combo_->setToolTip(lang.name);

  // Write to settings.
  settings_model_->setDefaultLocale(lang.locale);
}

}  // namespace installer