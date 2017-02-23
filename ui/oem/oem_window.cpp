// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/oem/oem_window.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QSplitter>
#include <QStringListModel>
#include <QVBoxLayout>

#include "service/settings_name.h"
#include "ui/oem/language_list_model.h"
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
  // Language selection
  connect(default_locale_combo_,
          static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &OemWindow::onDefaultLocaleIndexChanged);
}

void OemWindow::initUI() {
  // Pages
  QCheckBox* disk_space_box = settings_model_->addCheckBox(
      kSkipDiskSpaceInsufficientPage);
  disk_space_box->setText(tr("Skip disk space insufficient page"));
  QCheckBox* virtual_machine_box = settings_model_->addCheckBox(
      kSkipVirtualMachinePage);
  virtual_machine_box->setText(tr("Skip virtual machine warning page"));
  QCheckBox* skip_language_box = settings_model_->addCheckBox(
      kSkipSelectLanguagePage);
  skip_language_box->setText(tr("Skip language selection page"));
  QCheckBox* system_info_box = settings_model_->addCheckBox(
      kSkipSystemInfoPage);
  system_info_box->setText(tr("Skip system info page"));

  QCheckBox* partition_box = settings_model_->addCheckBox(kSkipPartitionPage);
  partition_box->setText(tr("Skip partition page"));

  // Language selection
  QLabel* default_locale_label = new QLabel(this);
  default_locale_label->setObjectName("default_locale_label");
  default_locale_label->setText(tr("Default locale:"));

  default_locale_combo_ = new QComboBox(this);
  default_locale_combo_->setObjectName("default_locale_combo");
  language_model_ = new LanguageListModel(true, this);
  default_locale_combo_->setModel(language_model_);
  // Select default locale.
  const QString default_locale = settings_model_->defaultLocale();
  const QModelIndex locale_index = language_model_->localeIndex(default_locale);
  if (locale_index.isValid()) {
    default_locale_combo_->setCurrentIndex(locale_index.row());
    const LanguageItem lang = language_model_->languageItemAt(locale_index);
    default_locale_combo_->setToolTip(lang.name);
  }

  QHBoxLayout* default_locale_layout = new QHBoxLayout();
  default_locale_layout->addWidget(default_locale_label);
  default_locale_layout->addWidget(default_locale_combo_);

  // System info
  QLineEdit* vendor_name_edit_ = settings_model_->addLineEdit(
      kSystemInfoVendorName);
  vendor_name_edit_->setPlaceholderText(tr("Vendor name"));

  QCheckBox* lock_username_box_ = settings_model_->addCheckBox(
      kSystemInfoLockUsername);
  lock_username_box_->setText(tr("Lock username"));
  QLineEdit* default_username_edit_ = settings_model_->addLineEdit(
      kSystemInfoDefaultUsername);
  default_username_edit_->setPlaceholderText(tr("Default username"));

  QCheckBox* lock_hostname_box_ = settings_model_->addCheckBox(
      kSystemInfoLockHostname);
  lock_hostname_box_->setText(tr("Lock hostname"));
  QLineEdit* default_hostname_edit_ = settings_model_->addLineEdit(
      kSystemInfoDefaultHostname);
  default_hostname_edit_->setPlaceholderText(tr("Default hostname"));

  QCheckBox* lock_password_box_ = settings_model_->addCheckBox(
      kSystemInfoLockPassword);
  lock_password_box_->setText(tr("Lock password"));
  QLineEdit* default_password_edit_ = settings_model_->addLineEdit(
      kSystemInfoDefaultPassword);
  default_password_edit_->setPlaceholderText(tr("Default password"));

  // Partition
  QCheckBox* skip_simple_part = settings_model_->addCheckBox(
      kPartitionSkipSimplePartitionPage);
  skip_simple_part->setText(tr("Skip simple partition page"));
  QCheckBox* skip_adv_part = settings_model_->addCheckBox(
      kPartitionSkipAdvancedPartitionPage);
  skip_adv_part->setText(tr("Skip advanced partition page"));

  default_fs_combo_ = new QComboBox();
  default_fs_combo_->addItems({"btrfs", "ext4", "ext3", "xfs"});

  QCheckBox* auto_part = settings_model_->addCheckBox(kPartitionDoAutoPart);
  auto_part->setText("Enable auto partition");

  QLineEdit* mount_points = settings_model_->addLineEdit(kPartitionMountPoints);
  mount_points->setPlaceholderText(tr("Supported mount points"));
  mount_points->setToolTip(mount_points->placeholderText());
  QLineEdit* formatted_mount_points = settings_model_->addLineEdit(
      kPartitionFormattedMountPoints);
  formatted_mount_points->setPlaceholderText(
      tr("Mount points to be formatted"));
  formatted_mount_points->setToolTip(formatted_mount_points->placeholderText());

    // Install failed frame
  QSpinBox* qr_err_msg = settings_model_->addSpinBox(kInstallFailedQRErrMsgLen,
                                                     100, 500);

  // Packages
  QLineEdit* uninstalled_packages = settings_model_->addLineEdit(
      kPackageUninstalledPackages);
  uninstalled_packages->setPlaceholderText("packages to be uninstalled");
  uninstalled_packages->setToolTip(uninstalled_packages->placeholderText());

  // Grub
  QLabel* grub_timeout_label = new QLabel();
  grub_timeout_label->setText(tr("Grub menu timeout:"));
  QSpinBox* grub_timeout_slider_ = settings_model_->addSpinBox(kGrubTimeout,
                                                              0, 30);
  QHBoxLayout* grub_timeout_layout = new QHBoxLayout();
  grub_timeout_layout->addWidget(grub_timeout_label);
  grub_timeout_layout->addWidget(grub_timeout_slider_);
  QCheckBox* block_windows = settings_model_->addCheckBox(kGrubBlockWindows);
  block_windows->setText(tr("Disable Windows in grub menu"));

  // Desktop
  QLineEdit* dde_dock_app_list = settings_model_->addLineEdit(kDdeDockAppList);
  dde_dock_app_list->setPlaceholderText(tr("apps to be added to dock"));
  dde_dock_app_list->setToolTip(dde_dock_app_list->placeholderText());
  QLineEdit* dde_launcher_app_list = settings_model_->addLineEdit(
      kDdeLauncherAppList);
  dde_launcher_app_list->setPlaceholderText(tr("apps to be added to launcher"));
  dde_launcher_app_list->setToolTip(dde_launcher_app_list->placeholderText());
  QLineEdit* dde_launcher_hold_packages = settings_model_->addLineEdit(
      kDdeLauncherHoldPackages);
  dde_launcher_hold_packages->setPlaceholderText(
      tr("apps to be prevented from uninstall in launcher"));
  dde_launcher_hold_packages->setToolTip(
      dde_launcher_hold_packages->placeholderText());
  QLineEdit* dde_desktop_content = settings_model_->addLineEdit(
      kDdeDesktopDesktopContent);
  dde_desktop_content->setPlaceholderText(tr("Set desktop content"));
  dde_desktop_content->setToolTip(dde_desktop_content->placeholderText());
  QLineEdit* dde_sound_effects = settings_model_->addLineEdit(
      kDdeSoundEffectSwitches);
  dde_sound_effects->setPlaceholderText(tr("Set sound effect switches"));
  dde_sound_effects->setToolTip(dde_sound_effects->placeholderText());

  // Services
  QLineEdit* enabled_services = settings_model_->addLineEdit(
      kServiceEnabledServices);
  enabled_services->setPlaceholderText(tr("enabled services"));
  enabled_services->setToolTip(enabled_services->placeholderText());
  QLineEdit* disabled_services = settings_model_->addLineEdit(
      kServiceDisabledServices);
  disabled_services->setPlaceholderText(tr("disabled services"));
  disabled_services->setToolTip(disabled_services->placeholderText());

  QVBoxLayout* right_layout = new QVBoxLayout();
  right_layout->addWidget(disk_space_box);
  right_layout->addWidget(virtual_machine_box);
  right_layout->addWidget(skip_language_box);
  right_layout->addWidget(system_info_box);
  right_layout->addWidget(partition_box);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addLayout(default_locale_layout);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(vendor_name_edit_);
  right_layout->addWidget(lock_username_box_);
  right_layout->addWidget(default_username_edit_);
  right_layout->addWidget(lock_hostname_box_);
  right_layout->addWidget(default_hostname_edit_);
  right_layout->addWidget(lock_password_box_);
  right_layout->addWidget(default_password_edit_);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(skip_simple_part);
  right_layout->addWidget(skip_adv_part);
  right_layout->addWidget(default_fs_combo_);
  right_layout->addWidget(auto_part);
  right_layout->addWidget(mount_points);
  right_layout->addWidget(formatted_mount_points);

  // Install progress frame
  right_layout->addSpacing(kSectionSpace);

  // Install failed frame
  right_layout->addWidget(qr_err_msg);

  // Packages
  right_layout->addWidget(uninstalled_packages);

  // Grub
  right_layout->addSpacing(kSectionSpace);
  right_layout->addLayout(grub_timeout_layout);
  right_layout->addWidget(block_windows);

  // Desktop
  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(dde_dock_app_list);
  right_layout->addWidget(dde_launcher_app_list);
  right_layout->addWidget(dde_launcher_hold_packages);
  right_layout->addWidget(dde_desktop_content);
  right_layout->addWidget(dde_sound_effects);

  right_layout->addSpacing(kSectionSpace);
  right_layout->addWidget(enabled_services);
  right_layout->addWidget(disabled_services);

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