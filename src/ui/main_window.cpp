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

#include "ui/main_window.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QShortcut>
#include <QStackedLayout>
#include <QTranslator>

#include "base/file_util.h"
#include "service/power_manager.h"
#include "service/screen_brightness.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/users.h"
#include "sysinfo/virtual_machine.h"
#include "third_party/global_shortcut/global_shortcut.h"
#include "ui/delegates/language_delegate.h"
#include "ui/delegates/main_window_util.h"
#include "ui/frames/confirm_quit_frame.h"
#include "ui/frames/control_panel_frame.h"
#include "ui/frames/disk_space_insufficient_frame.h"
#include "ui/frames/install_failed_frame.h"
#include "ui/frames/install_progress_frame.h"
#include "ui/frames/install_success_frame.h"
#include "ui/frames/partition_frame.h"
#include "ui/frames/privilege_error_frame.h"
#include "ui/frames/language_frame.h"
#include "ui/frames/system_info_frame.h"
#include "ui/frames/timezone_frame.h"
#include "ui/frames/virtual_machine_frame.h"

#include "ui/utils/widget_util.h"
#include "ui/widgets/page_indicator.h"
#include "ui/widgets/pointer_button.h"
#include "ui/xrandr/multi_head_manager.h"

namespace installer {

MainWindow::MainWindow()
    : QWidget(),
      pages_(),
      prev_page_(PageId::NullId),
      current_page_(PageId::NullId),
      log_file_(),
      auto_install_(false) {
  this->setObjectName("main_window");

  this->initUI();
  this->initPages();
  this->registerShortcut();
  this->initConnections();

    page_indicator_->updatePages(GetVisiblePages());

  SetBrightness(GetSettingsInt(kScreenDefaultBrightness));
    WriteDisplayPort(getenv("DISPLAY"));
}

void MainWindow::fullscreen() {
  if (auto_install_) {
    // Read default locale from settings.ini and go to InstallProgressFrame.
    current_page_ = PageId::PartitionId;

    // Set language.
    QTranslator* translator = new QTranslator(this);
    const QString locale(ReadLocale());
    translator->load(GetLocalePath(locale));
    qApp->installTranslator(translator);
  }

#ifndef QT_DEBUG
    multi_head_manager_->updateWallpaper();
#endif // !QT_DEBUG

  ShowFullscreen(this);
  this->goNextPage();

  if (auto_install_) {
    // In auto-install mode, partitioning is done in hook script.
    // So notify InstallProgressFrame to run hooks directly.
    emit partition_frame_->autoPartDone(true);
  }
}

void MainWindow::scanDevicesAndTimezone() {
  // Do nothing in auto-install mode.
  if (auto_install_) {
    return;
  }

  // If timezone page is not skipped, scan wireless hot spot and update
  // timezone in background.
  if (!GetSettingsBool(kSkipTimezonePage)) {
    timezone_frame_->readConf();
  }

  if (!GetSettingsBool(kSkipPartitionPage) &&
      !GetSettingsBool(kPartitionDoAutoPart)) {
    // Notify background thread to scan device list.
    // When device is refreshed in partition_frame_, call fullscreen() method
    // to display main window.
    partition_frame_->scanDevices();
  }
}

void MainWindow::setEnableAutoInstall(bool auto_install) {
  auto_install_ = auto_install;
}

void MainWindow::setLogFile(const QString& log_file) {
  log_file_ = log_file;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  this->updateBackground();
  QWidget::resizeEvent(event);
}

void MainWindow::initConnections() {
  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitCancelled,
          this, &MainWindow::goNextPage);
  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitConfirmed,
          this, &MainWindow::shutdownSystem);

  connect(control_panel_frame_, &ControlPanelFrame::currentPageChanged,
          this, &MainWindow::onCurrentPageChanged);
  connect(control_panel_frame_, &ControlPanelFrame::requestRefreshDevices,
          partition_frame_, &PartitionFrame::scanDevices);
  connect(control_panel_frame_, &ControlPanelFrame::requestRefreshQR,
          install_failed_frame_, &InstallFailedFrame::updateMessage);
  connect(control_panel_frame_, &ControlPanelFrame::requestSimulateSlide,
          install_progress_frame_, &InstallProgressFrame::simulate);

  connect(disk_space_insufficient_frame_, &DiskSpaceInsufficientFrame::finished,
          this, &MainWindow::shutdownSystem);

  connect(install_failed_frame_, &InstallFailedFrame::finished,
          this, &MainWindow::shutdownSystem);

  connect(install_progress_frame_, &InstallProgressFrame::finished,
          this, &MainWindow::goNextPage);

  if (GetSettingsBool(kSystemInfoSetupAfterReboot)) {
      connect(install_success_frame_, &InstallSuccessFrame::finished,
              this, &MainWindow::shutdownSystem);
  } else {
      connect(install_success_frame_, &InstallSuccessFrame::finished,
              this, &MainWindow::rebootSystem);
  }

  connect(partition_frame_, &PartitionFrame::reboot,
          this, &MainWindow::rebootSystem);
  connect(partition_frame_, &PartitionFrame::finished,
          this, &MainWindow::goNextPage);

  connect(privilege_error_frame_, &PrivilegeErrorFrame::finished,
          this, &MainWindow::goNextPage);

  connect(select_language_frame_, &LanguageFrame::finished,
          this, &MainWindow::goNextPage);
  connect(system_info_frame_, &SystemInfoFrame::finished,
          this, &MainWindow::goNextPage);

  connect(select_language_frame_, &LanguageFrame::timezoneUpdated,
          timezone_frame_, &TimezoneFrame::updateTimezoneBasedOnLanguage);
  connect(timezone_frame_, &TimezoneFrame::finished,
          this, &MainWindow::goNextPage);

  connect(virtual_machine_frame_, &VirtualMachineFrame::finished,
          this, &MainWindow::goNextPage);

  // Notify InstallProgressFrame that partition job has finished.
  connect(partition_frame_, &PartitionFrame::autoPartDone,
          install_progress_frame_, &InstallProgressFrame::runHooks);
  connect(partition_frame_, &PartitionFrame::manualPartDone,
          install_progress_frame_, &InstallProgressFrame::runHooks);

  connect(close_button_, &QPushButton::clicked,
          this, &MainWindow::onCloseButtonClicked);
  connect(multi_head_manager_, &MultiHeadManager::primaryScreenChanged,
          this, &MainWindow::onPrimaryScreenChanged);

  connect(control_panel_shortcut_, &QShortcut::activated,
          control_panel_frame_, &ControlPanelFrame::toggleVisible);
  connect(monitor_mode_shortcut_, &GlobalShortcut::activated,
          multi_head_manager_, &MultiHeadManager::switchXRandRMode);
  connect(brightness_increase_shortcut_, &QShortcut::activated,
          IncreaseBrightness);
  connect(brithtness_decrease_shortcut_, &QShortcut::activated,
          DecreaseBrightness);
}

void MainWindow::initPages() {
  confirm_quit_frame_ = new ConfirmQuitFrame(this);
  pages_.insert(PageId::ConfirmQuitId,
               stacked_layout_->addWidget(confirm_quit_frame_));

  select_language_frame_ = new LanguageFrame(this);
  pages_.insert(PageId::SelectLanguageId,
                stacked_layout_->addWidget(select_language_frame_));

  disk_space_insufficient_frame_ = new DiskSpaceInsufficientFrame(this);
  pages_.insert(PageId::DiskSpaceInsufficientId,
                stacked_layout_->addWidget(disk_space_insufficient_frame_));

  install_failed_frame_ = new InstallFailedFrame(this);
  pages_.insert(PageId::InstallFailedId,
                stacked_layout_->addWidget(install_failed_frame_));

  install_progress_frame_ = new InstallProgressFrame(this);
  pages_.insert(PageId::InstallProgressId,
                stacked_layout_->addWidget(install_progress_frame_));

  install_success_frame_ = new InstallSuccessFrame(this);
  pages_.insert(PageId::InstallSuccessId,
                stacked_layout_->addWidget(install_success_frame_));

  partition_frame_ = new PartitionFrame(this);
  pages_.insert(PageId::PartitionId,
                stacked_layout_->addWidget(partition_frame_));

  privilege_error_frame_ = new PrivilegeErrorFrame(this);
  pages_.insert(PageId::PrivilegeErrorId,
                stacked_layout_->addWidget(privilege_error_frame_));

  system_info_frame_ = new SystemInfoFrame(this);
  pages_.insert(PageId::SystemInfoId,
                stacked_layout_->addWidget(system_info_frame_));

  timezone_frame_ = new TimezoneFrame(this);
  pages_.insert(PageId::TimezoneId,
                stacked_layout_->addWidget(timezone_frame_));

  virtual_machine_frame_ = new VirtualMachineFrame(this);
  pages_.insert(PageId::VirtualMachineId,
                stacked_layout_->addWidget(virtual_machine_frame_));
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);

  close_button_ = new PointerButton();
  close_button_->setObjectName("close_button");
  close_button_->setFlat(true);
  close_button_->setFocusPolicy(Qt::TabFocus);
  close_button_->setFixedSize(40, 40);
  close_button_->setStyleSheet(ReadFile(":/styles/close_button.css"));
  QHBoxLayout* close_layout = new QHBoxLayout();
  close_layout->setContentsMargins(0, 0, 0, 0);
  close_layout->setSpacing(0);
  close_layout->addStretch();
  close_layout->addWidget(close_button_);
  QFrame* close_button_wrapper = new QFrame();
  // Add 4px at top and right margin.
  close_button_wrapper->setContentsMargins(0, 4, 4, 0);
  close_button_wrapper->setLayout(close_layout);

  stacked_layout_ = new QStackedLayout();

  // Use a wrapper to hold its position.
  QFrame* page_indicator_wrapper = new QFrame();
  page_indicator_wrapper->setFixedHeight(48);
  page_indicator_ = new PageIndicator(GetVisiblePages(),
                                      page_indicator_wrapper);
  QHBoxLayout* indicator_layout = new QHBoxLayout();
  indicator_layout->addWidget(page_indicator_);
  page_indicator_wrapper->setLayout(indicator_layout);

  QVBoxLayout* vbox_layout = new QVBoxLayout();
  vbox_layout->setContentsMargins(0, 0, 0, 0);
  vbox_layout->setSpacing(0);
  vbox_layout->addWidget(close_button_wrapper);
  vbox_layout->addLayout(stacked_layout_);
  vbox_layout->addWidget(page_indicator_wrapper);
  vbox_layout->addSpacing(32);

  this->setLayout(vbox_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setWindowFlags(Qt::FramelessWindowHint);

  control_panel_frame_ = new ControlPanelFrame(this);
  control_panel_frame_->hide();

  multi_head_manager_ = new MultiHeadManager(this);
}

void MainWindow::registerShortcut() {
  control_panel_shortcut_ = new QShortcut(QKeySequence("Ctrl+L"), this);
  control_panel_shortcut_->setContext(Qt::ApplicationShortcut);
  // Note(xushaohua): Super key is named Meta key in Qt namespace.
  monitor_mode_shortcut_ = new GlobalShortcut(QKeySequence("Meta+P"), this);
  if (!monitor_mode_shortcut_->registerNow()) {
    qWarning() << "Failed to register global shortcut of Meta+P"
               << "Fallback to Ctrl+Alt+P";
    delete monitor_mode_shortcut_;
    monitor_mode_shortcut_ = new GlobalShortcut(QKeySequence("Ctrl+Alt+P"),
                                                this);
    if (!monitor_mode_shortcut_->registerNow()) {
      qWarning() << "Failed to register global shortcut of Ctrl+Alt+P";
    }
  }

  brightness_increase_shortcut_ = new QShortcut(QKeySequence("Ctrl+="), this);
  brithtness_decrease_shortcut_ = new QShortcut(QKeySequence("Ctrl+-"), this);
}

void MainWindow::saveLogFile() {
  if (!log_file_.isEmpty()) {
    // Copy log file.
    CopyLogFile(log_file_);
  }
}

void MainWindow::setCurrentPage(PageId page_id) {
  Q_ASSERT(pages_.contains(page_id));
  Q_ASSERT(page_id != PageId::NullId);

  prev_page_ = current_page_;
  current_page_ = page_id;
  stacked_layout_->setCurrentIndex(pages_.value(page_id));
  if (page_id == PageId::ConfirmQuitId ||
      page_id == PageId::InstallProgressId ||
      page_id == PageId::InstallSuccessId ||
      page_id == PageId::InstallFailedId) {
    // Hide close button in ConfirmQuit page and InstallProgress page
    close_button_->hide();
  } else {
    close_button_->show();
  }

  if (page_id == PageId::InstallFailedId ||
      page_id == PageId::InstallSuccessId) {
    // Hide page indicator in these pages.
    page_indicator_->hide();
  } else {
    page_indicator_->show();
  }

  // Raise control_panel_frame explicitly.
  if (control_panel_frame_->isVisible()) {
    control_panel_frame_->raise();
  }
}

void MainWindow::updateBackground() {
  if (!background_label_) {
    qWarning() << "background_label is not initialized!";
    return;
  }
  const QString image_path = GetWindowBackground();
  // Other platforms may have performance issue.
  const QPixmap pixmap =
      QPixmap(image_path).scaled(size(), Qt::KeepAspectRatioByExpanding);
  background_label_->setPixmap(pixmap);
  background_label_->setFixedSize(size());
}

void MainWindow::onCurrentPageChanged(int index) {
  // Ignore null id.
  const PageId id = static_cast<PageId>(index + 1);
  this->setCurrentPage(id);
}

void MainWindow::onCloseButtonClicked() {
  this->setCurrentPage(PageId::ConfirmQuitId);
}

void MainWindow::onPrimaryScreenChanged(const QRect& geometry) {
  qDebug() << "onPrimaryScreenChanged()" << geometry;
  ShowFullscreen(this, geometry);
}

void MainWindow::goNextPage() {
    // Page order:
    //   * privilege error frame;
    //   * select language frame;
    //   * disk space insufficient page;
    //   * virtual machine page;
    //   * system info page;
    //   * timezone page;
    //   * partition page;
    //   * install progress page;
    //   * install success page or install failed page;
    // And confirm-quit-page can be triggered at any moment except in
    // install progress page.
    switch (current_page_) {
    case PageId::ConfirmQuitId: {
        // Go previous page.
        this->setCurrentPage(prev_page_);
        break;
    }

    case PageId::NullId: {
        if (HasRootPrivilege()) {
            current_page_ = PageId::PrivilegeErrorId;
        } else {
            this->setCurrentPage(PageId::PrivilegeErrorId);
            break;
        }
    }

    case PageId::PrivilegeErrorId: {
        select_language_frame_->readConf();
        if (GetSettingsBool(kSkipSelectLanguagePage)) {
            select_language_frame_->writeConf();
            prev_page_ = current_page_;
            current_page_ = PageId::SelectLanguageId;
        } else {
            page_indicator_->goNextPage();
            this->setCurrentPage(PageId::SelectLanguageId);
            break;
        }
    }

    case PageId::SelectLanguageId: {
        // Check whether to show DiskSpaceInsufficientPage.
        if (!GetSettingsBool(kSkipDiskSpaceInsufficientPage) &&
                IsDiskSpaceInsufficient()) {
            page_indicator_->goNextPage();
            this->setCurrentPage(PageId::DiskSpaceInsufficientId);
            break;
        }
        else {
            prev_page_ = current_page_;
            current_page_ = PageId::DiskSpaceInsufficientId;
        }
    }

    case PageId::DiskSpaceInsufficientId: {
        // Check whether to show VirtualMachinePage.
        if (!GetSettingsBool(kSkipVirtualMachinePage) &&
                IsVirtualMachine()) {
            this->setCurrentPage(PageId::VirtualMachineId);
            break;
        } else {
            prev_page_ = current_page_;
            current_page_ = PageId::VirtualMachineId;
        }
    }

    case PageId::VirtualMachineId: {
        // Check whether to show SystemInfoPage.
        system_info_frame_->readConf();
        if (GetSettingsBool(kSystemInfoSetupAfterReboot) || GetSettingsBool(kSkipSystemInfoPage)) {
            system_info_frame_->writeConf();
            prev_page_ = current_page_;
            current_page_ = PageId::SystemInfoId;
        } else {
            page_indicator_->goNextPage();
            this->setCurrentPage(PageId::SystemInfoId);
            break;
        }
    }

    case PageId::SystemInfoId: {
        if (GetSettingsBool(kSystemInfoSetupAfterReboot) || GetSettingsBool(kSkipTimezonePage)) {
            timezone_frame_->writeConf();
            prev_page_ = current_page_;
            current_page_ = PageId::TimezoneId;
        } else {
            page_indicator_->goNextPage();
            this->setCurrentPage(PageId::TimezoneId);
            break;
        }
    }

    case PageId::TimezoneId: {
        // Check whether to show PartitionPage.
        if (GetSettingsBool(kSkipPartitionPage)) {
            if (GetSettingsBool(kPartitionDoAutoPart)) {
                partition_frame_->autoPart();
            }
            prev_page_ = current_page_;
            current_page_ = PageId::PartitionId;
        } else {
            page_indicator_->goNextPage();
            this->setCurrentPage(PageId::PartitionId);
            break;
        }
    }

    case PageId::PartitionId: {
        // Show InstallProgressFrame.
        page_indicator_->goNextPage();
        install_progress_frame_->startSlide();
        this->setCurrentPage(PageId::InstallProgressId);
        break;
    }

    case PageId::InstallProgressId: {
        if (install_progress_frame_->failed()) {
            install_failed_frame_->updateMessage();
            this->setCurrentPage(PageId::InstallFailedId);
        } else {
            install_success_frame_->setEjectLabelVisible(!auto_install_);
            this->setCurrentPage(PageId::InstallSuccessId);
        }
        break;
    }

    default: {
        qWarning() << "[MainWindow]::goNextPage() We shall never reach here"
                   << static_cast<int>(current_page_) << this->sender();
        break;
    }
    }
}

void MainWindow::rebootSystem() {
  this->saveLogFile();

  if (!RebootSystemWithMagicKey()) {
    qWarning() << "RebootSystem failed!";
    if (!RebootSystem()) {
      qWarning() << "RebootSystemWithMagicKey() failed!";
    }
  }
}

void MainWindow::shutdownSystem() {
  this->saveLogFile();

  if (!ShutdownSystemWithMagicKey()) {
    qWarning() << "ShutdownSystem() failed!";
    if (!ShutdownSystem()) {
      qWarning() << "ShutdownSystemWithMagicKey() failed!";
    }
  }
}

}  // namespace installer
