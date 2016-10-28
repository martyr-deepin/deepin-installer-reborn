// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QApplication>
#include <QDebug>
#include <QHash>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QResizeEvent>
#include <QShortcut>
#include <QStackedLayout>

#include "partman/partition_manager.h"
#include "partman/utils.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/virtual_machine.h"
#include "ui/frames/confirm_quit_frame.h"
#include "ui/frames/disk_space_insufficient_frame.h"
#include "ui/frames/install_failed_frame.h"
#include "ui/frames/install_progress_frame.h"
#include "ui/frames/install_success_frame.h"
#include "ui/frames/log_viewer_frame.h"
#include "ui/frames/partition_frame.h"
#include "ui/frames/partition_table_warning_frame.h"
#include "ui/frames/select_language_frame.h"
#include "ui/frames/system_info_frame.h"
#include "ui/frames/virtual_machine_frame.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/page_indicator.h"
#include "ui/xrandr/multi_head_manager.h"

namespace installer {

namespace {

int GetVisiblePages() {
  int pages = 0;
  if (!GetSettingsBool(kSkipSelectLanguagePage)) {
    pages += 1;
  }
  if (!GetSettingsBool(kSkipSystemInfoPage)) {
    pages += 1;
  }
  if (!GetSettingsBool(kSkipPartitionPage)) {
    pages += 1;
  }
  // For install-progress page.
  pages += 1;
  return pages;
}

// Check size of /dev/sda larger than 15Gib.
bool IsDiskSpaceInsufficient(int required_device_size) {
  const qint64 maximum_device_size = GetMaximumDeviceSize();
  return required_device_size * kMebiByte > maximum_device_size;
}

// Check whether partition table matches machine settings.
bool IsPartitionTableMatch() {
  // If EFI is not enabled, always returns true.
  if (!IsEfiEnabled()) {
    return true;
  }

  PartitionTableType type = GetPrimaryDiskPartitionTable();

  // If partition table is empty(a raw disk device), returns true.
  if (type == PartitionTableType::Empty) {
    return true;
  }

  return type == PartitionTableType::GPT;
}

}  // namespace

MainWindow::MainWindow()
    : QWidget(),
      pages_(),
      prev_page_(PageId::NullId),
      current_page_(PageId::NullId) {
  this->setObjectName(QStringLiteral("main_window"));

  wallpaper_manager_ = new MultiHeadManager(this);

  this->initUI();
  this->initPages();
  this->registerShortcut();
  this->initConnections();
  current_page_ = PageId::PartitionId;
  this->goNextPage();
}

void MainWindow::fullscreen() {
  wallpaper_manager_->updateWallpaper();
  this->showFullScreen();
}

void MainWindow::scanDevices() {
  partition_frame_->scanDevices();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  this->updateBackground();

  QWidget::resizeEvent(event);
}

void MainWindow::initConnections() {
  connect(close_button_, &QPushButton::clicked,
          this, &MainWindow::onCloseButtonClicked);

  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitCancelled,
          this, &MainWindow::goNextPage);
  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitConfirmed,
          this, &MainWindow::rebootSystem);
  connect(disk_space_insufficient_frame_, &DiskSpaceInsufficientFrame::finished,
          this, &MainWindow::rebootSystem);
  connect(install_failed_frame_, &InstallFailedFrame::finished,
          this, &MainWindow::rebootSystem);
  connect(install_progress_frame_, &InstallProgressFrame::finished,
          this, &MainWindow::goNextPage);
  connect(install_success_frame_, &InstallSuccessFrame::finished,
          this, &MainWindow::rebootSystem);
  connect(partition_frame_, &PartitionFrame::finished,
          this, &MainWindow::goNextPage);
  connect(partition_table_warning_frame_, &PartitionTableWarningFrame::declined,
          this, &MainWindow::rebootSystem);
  connect(partition_table_warning_frame_, &PartitionTableWarningFrame::accepted,
          this, &MainWindow::goNextPage);
  connect(select_language_frame_, &SelectLanguageFrame::finished,
          this, &MainWindow::goNextPage);
  connect(system_info_frame_, &SystemInfoFrame::finished,
          this, &MainWindow::goNextPage);
  connect(virtual_machine_frame_, &VirtualMachineFrame::finished,
          this, &MainWindow::goNextPage);

  connect(log_viewer_shortcut_, &QShortcut::activated,
          log_viewer_frame_, &LogViewerFrame::toggleVisible);
  connect(monitor_mode_shortcut_, &QShortcut::activated,
          wallpaper_manager_, &MultiHeadManager::switchXRandRMode);
  connect(wallpaper_manager_, &MultiHeadManager::primaryScreenChanged,
          this, &MainWindow::onPrimaryScreenChanged);

  // Notify InstallProgressFrame that partition job has finished.
  connect(partition_frame_, &PartitionFrame::autoPartDone,
          install_progress_frame_, &InstallProgressFrame::runHooks);
  connect(partition_frame_, &PartitionFrame::manualPartDone,
          install_progress_frame_, &InstallProgressFrame::runHooks);

  connect(select_language_frame_, &SelectLanguageFrame::languageSelected,
          install_progress_frame_, &InstallProgressFrame::updateLanguage);
}

void MainWindow::initPages() {
  confirm_quit_frame_ = new ConfirmQuitFrame(this);
  pages_.insert(PageId::ConfirmQuitId,
                stacked_layout_->addWidget(confirm_quit_frame_));

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

  partition_table_warning_frame_ = new PartitionTableWarningFrame(this);
  pages_.insert(PageId::PartitionTableWarningId,
                stacked_layout_->addWidget(partition_table_warning_frame_));

  select_language_frame_ = new SelectLanguageFrame(this);
  pages_.insert(PageId::SelectLanguageId,
                stacked_layout_->addWidget(select_language_frame_));

  system_info_frame_ = new SystemInfoFrame(this);
  pages_.insert(PageId::SystemInfoId,
                stacked_layout_->addWidget(system_info_frame_));

  virtual_machine_frame_ = new VirtualMachineFrame(this);
  pages_.insert(PageId::VirtualMachineId,
                stacked_layout_->addWidget(virtual_machine_frame_));

  log_viewer_frame_ = new LogViewerFrame();
  // TODO(xushaohua): Move to center of parent window.
  log_viewer_frame_->hide();
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);

  QFrame* close_button_wrapper = new QFrame();
  close_button_wrapper->setFixedHeight(36);
  close_button_ = new IconButton(QStringLiteral(":/images/close_normal.png"),
                                 QStringLiteral(":/images/close_hover.png"),
                                 QStringLiteral(":/images/close_press.png"),
                                 32, 32, close_button_wrapper);
  QHBoxLayout* close_layout = new QHBoxLayout();
  close_layout->setAlignment(Qt::AlignRight);
  close_layout->addWidget(close_button_);
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
  this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::registerShortcut() {
  log_viewer_shortcut_ = new QShortcut(QKeySequence("Ctrl+L"), this);
  log_viewer_shortcut_->setContext(Qt::ApplicationShortcut);

  monitor_mode_shortcut_ = new QShortcut(QKeySequence("Ctrl+P"), this);
  monitor_mode_shortcut_->setContext(Qt::ApplicationShortcut);
}

void MainWindow::setCurrentPage(PageId page_id) {
  Q_ASSERT(pages_.contains(page_id));
  Q_ASSERT(page_id != PageId::NullId);

  prev_page_ = current_page_;
  current_page_ = page_id;
  stacked_layout_->setCurrentIndex(pages_.value(page_id));
  if (page_id == PageId::ConfirmQuitId ||
      page_id == PageId::InstallProgressId) {
    // Hide close button in ConfirmQuit page and InstallProgress page
    close_button_->hide();
  } else {
    close_button_->show();
  }
}

void MainWindow::updateBackground() {
  if (!background_label_) {
    qWarning() << "background_label is not initialized!";
    return;
  }
  const QString image_path = GetWindowBackground();
  // TODO(xushaohua): Only scale background image in x86 architecture.
  // Other platforms may have performance issue.
  const QPixmap pixmap = QPixmap(image_path).scaled(
      size(), Qt::KeepAspectRatioByExpanding);
  background_label_->setPixmap(pixmap);
  background_label_->setFixedSize(size());
}

void MainWindow::onCloseButtonClicked() {
  this->setCurrentPage(PageId::ConfirmQuitId);
}

void MainWindow::onPrimaryScreenChanged(const QRect& geometry) {
  qDebug() << "MainWindow::onPrimaryScreenChanged()" << geometry;
  this->move(geometry.topLeft());
  this->setFixedSize(geometry.size());
}

void MainWindow::goNextPage() {
  // Page order:
  //   * disk space insufficient page;
  //   * virtual machine page;
  //   * partition table warning page;
  //   * select language page;
  //   * system info page;
  //   * partition page;
  //   * install progress page;
  //   * install success page or install failed page;
  // And confirm quit page can be displayed at any moment.
  switch (current_page_) {
    case PageId::ConfirmQuitId: {
      // Go previous page.
      this->setCurrentPage(prev_page_);
      break;
    }

    case PageId::DiskSpaceInsufficientId: {
      // Check whether to show VirtualMachinePage.
      if (IsVirtualMachine()) {
        page_indicator_->setVisible(false);
        this->setCurrentPage(PageId::VirtualMachineId);
      } else {
        prev_page_ = current_page_;
        current_page_ = PageId::VirtualMachineId;
        this->goNextPage();
      }
      break;
    }

    case PageId::InstallProgressId: {
      page_indicator_->setVisible(false);
      if (install_progress_frame_->failed()) {
        install_failed_frame_->updateErrorMessage();
        this->setCurrentPage(PageId::InstallFailedId);
      } else {
        this->setCurrentPage(PageId::InstallSuccessId);
      }
      break;
    }

    case PageId::PartitionId: {
      // Show InstallProgressPage.
      page_indicator_->setVisible(true);
      page_indicator_->goNextPage();
      install_progress_frame_->startSlide();
      this->setCurrentPage(PageId::InstallProgressId);
      break;
    }

    case PageId::PartitionTableWarningId: {
      // Check whether to show SelectLanguagePage.
      if (!GetSettingsBool(kSkipSelectLanguagePage)) {
        page_indicator_->setVisible(true);
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::SelectLanguageId);
      } else {
        select_language_frame_->autoConf();
        prev_page_ = current_page_;
        current_page_ = PageId::SelectLanguageId;
        this->goNextPage();
      }
      break;
    }

    case PageId::SelectLanguageId: {
      // Check whether to show SystemInfoPage.
      if (!GetSettingsBool(kSkipSystemInfoPage)) {
        page_indicator_->setVisible(true);
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::SystemInfoId);
      } else {
        system_info_frame_->autoConf();
        prev_page_ = current_page_;
        current_page_ = PageId::SystemInfoId;
        this->goNextPage();
      }
      break;
    }

    case PageId::SystemInfoId: {
      // Check whether to show PartitionPage.
      if (!GetSettingsBool(kSkipPartitionPage)) {
        page_indicator_->setVisible(true);
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::PartitionId);
      } else {
        if (GetSettingsBool(kPartitionDoAutoPart)) {
          partition_frame_->autoPart();
        }
        prev_page_ = current_page_;
        current_page_ = PageId::PartitionId;
        this->goNextPage();
      }
      break;
    }

    case PageId::VirtualMachineId: {
      // Check whether to show PartitionTableWarningPage.
      if (!IsPartitionTableMatch()) {
        page_indicator_->setVisible(false);
        this->setCurrentPage(PageId::PartitionTableWarningId);
      } else {
        prev_page_ = current_page_;
        current_page_ = PageId::PartitionTableWarningId;
        this->goNextPage();
      }
      break;
    }

    case PageId::NullId: {
      // Displays the first page.
      // Check whether to show DiskSpaceInsufficientPage.
      const int required_device_size = GetSettingsValue(
          kPartitionMinimumDiskSpaceRequired).toInt();
      if (IsDiskSpaceInsufficient(required_device_size)) {
        page_indicator_->setVisible(false);
        const int recommended_device_size = GetSettingsValue(
            kPartitionRecommendedDiskSpace).toInt();
        disk_space_insufficient_frame_->setSizes(required_device_size,
                                                 recommended_device_size);
        this->setCurrentPage(PageId::DiskSpaceInsufficientId);
      } else {
        prev_page_ = current_page_;
        current_page_ = PageId::DiskSpaceInsufficientId;
        this->goNextPage();
      }
      break;
    }

    default: {
      qWarning() << "[MainWindow]::goNextPage() We shall never reach here";
      break;
    }
  }
}

void MainWindow::rebootSystem() {
  // TODO(xushaohua): reboot system
  qApp->quit();
}

void MainWindow::shutdownSystem() {
  // TODO(xushaohua): Poweroff system.
  this->close();
}

}  // namespace installer
