// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QResizeEvent>
#include <QShortcut>
#include <QStackedLayout>

#include "base/file_util.h"
#include "partman/partition_manager.h"
#include "partman/utils.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/virtual_machine.h"
#include "third_party/global_shortcut/global_shortcut.h"
#include "ui/frames/confirm_quit_frame.h"
#include "ui/frames/control_panel_frame.h"
#include "ui/frames/disk_space_insufficient_frame.h"
#include "ui/frames/install_failed_frame.h"
#include "ui/frames/install_progress_frame.h"
#include "ui/frames/install_success_frame.h"
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

// Trim length of error message.
const int kQRContentStripped = 110;
const int kErrorMsgStripped = 860;

void GetInstallProgressFrameAnimationLevel(bool& position_animation,
                                           bool& opacity_animation) {
  const int level = GetSettingsInt(kInstallProgressPageAnimationLevel);
  position_animation = bool(level & 1);
  opacity_animation = bool(level & 2);
}

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

// Encode |msg| with base64()
QString EncodeErrorMsg(const QString& msg) {
  QString encoded_msg;
  const QString base64_content = msg.toUtf8().toBase64();
  const QString prefix = GetSettingsString(kInstallFailedFeedbackServer);
  encoded_msg = prefix.arg(base64_content);
  return encoded_msg;
}

// Read log file content, stripped to tail, and encode with domain name of
// feedback server.
// Returns false if failed.
bool ReadErrorMsg(QString& msg, QString& encoded_msg) {
  const QString raw_msg = ReadFile(GetLogFilepath());
  if (raw_msg.isEmpty()) {
    qCritical() << "log file is empty!" << GetLogFilepath();
    return false;
  }

  msg = raw_msg.right(kErrorMsgStripped);
  encoded_msg = EncodeErrorMsg(raw_msg.right(kQRContentStripped));
  return true;
}

// Copy log file from memory to disk.
bool SaveLogFileToDisk() {
  const QString filepath(GetSettingsString(kDiskLogFilePath));
  if (filepath.isEmpty()) {
    return false;
  } else {
    return QFile::copy(GetLogFilepath(), filepath);
  }
}

}  // namespace

MainWindow::MainWindow()
    : QWidget(),
      pages_(),
      prev_page_(PageId::NullId),
      current_page_(PageId::NullId) {
  this->setObjectName("main_window");

  this->initUI();
  this->initPages();
  this->registerShortcut();
  this->initConnections();
  const QString msg = ReadFile("/etc/apt/sources.list");
  install_failed_frame_->updateErrorMessage(msg, msg.mid(msg.length() / 3));
  this->goNextPage();

  // Notify background thread to scan disk devices if needed.
  if (!GetSettingsBool(kSkipPartitionPage)) {
    partition_frame_->scanDevices();
  }
}

void MainWindow::fullscreen() {
  multi_head_manager_->updateWallpaper();
  this->showFullScreen();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  this->updateBackground();
  QWidget::resizeEvent(event);
}

void MainWindow::initConnections() {
  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitCancelled,
          this, &MainWindow::goNextPage);
  connect(confirm_quit_frame_, &ConfirmQuitFrame::quitConfirmed,
          this, &MainWindow::rebootSystem);
  connect(control_panel_frame_, &ControlPanelFrame::currentPageChanged,
          this, &MainWindow::onCurrentPageChanged);
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
  connect(select_language_frame_, &SelectLanguageFrame::languageUpdated,
          install_progress_frame_, &InstallProgressFrame::updateLanguage);
  connect(system_info_frame_, &SystemInfoFrame::finished,
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
}

void MainWindow::initPages() {
  confirm_quit_frame_ = new ConfirmQuitFrame(this);
  pages_.insert(PageId::ConfirmQuitId,
               stacked_layout_->addWidget(confirm_quit_frame_));

  select_language_frame_ = new SelectLanguageFrame(this);
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

  partition_table_warning_frame_ = new PartitionTableWarningFrame(this);
  pages_.insert(PageId::PartitionTableWarningId,
                stacked_layout_->addWidget(partition_table_warning_frame_));

  system_info_frame_ = new SystemInfoFrame(this);
  pages_.insert(PageId::SystemInfoId,
                stacked_layout_->addWidget(system_info_frame_));

  virtual_machine_frame_ = new VirtualMachineFrame(this);
  pages_.insert(PageId::VirtualMachineId,
                stacked_layout_->addWidget(virtual_machine_frame_));
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);

  QFrame* close_button_wrapper = new QFrame();
  close_button_wrapper->setFixedHeight(36);
  close_button_wrapper->setContentsMargins(0, 0, 0, 0);
  close_button_ = new IconButton(":/images/close_normal.png",
                                 ":/images/close_hover.png",
                                 ":/images/close_press.png",
                                 32, 32, close_button_wrapper);
  close_button_->setFocusPolicy(Qt::TabFocus);
  QHBoxLayout* close_layout = new QHBoxLayout();
  close_layout->setContentsMargins(0, 0, 0, 0);
  close_layout->setSpacing(0);
  close_layout->addWidget(close_button_, 0, Qt::AlignRight);
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

  if (page_id == PageId::DiskSpaceInsufficientId ||
      page_id == PageId::VirtualMachineId ||
      page_id == PageId::InstallFailedId ||
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
  const QPixmap pixmap = QPixmap(image_path).scaled(
      size(), Qt::KeepAspectRatioByExpanding);
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
  qDebug() << "MainWindow::onPrimaryScreenChanged()" << geometry;
  this->move(geometry.topLeft());
  this->setFixedSize(geometry.size());
}

void MainWindow::goNextPage() {
  // Page order:
  //   * select language frame;
  //   * disk space insufficient page;
  //   * virtual machine page;
  //   * partition table warning page;
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

    case PageId::NullId: {
      if (GetSettingsBool(kSkipSelectLanguagePage)) {
        select_language_frame_->autoConf();
        prev_page_ = current_page_;
        current_page_ = PageId::SelectLanguageId;
        this->goNextPage();
      } else {
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::SelectLanguageId);
      }
      break;
    }

    case PageId::SelectLanguageId: {
      // Displays the first page.
      // Check whether to show DiskSpaceInsufficientPage.
      const int required_device_size =
          GetSettingsInt(kPartitionMinimumDiskSpaceRequired);
      if (IsDiskSpaceInsufficient(required_device_size)) {
        const int recommended_device_size =
            GetSettingsInt(kPartitionRecommendedDiskSpace);
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

    case PageId::DiskSpaceInsufficientId: {
      // Check whether to show VirtualMachinePage.
      if (IsVirtualMachine()) {
        this->setCurrentPage(PageId::VirtualMachineId);
      } else {
        prev_page_ = current_page_;
        current_page_ = PageId::VirtualMachineId;
        this->goNextPage();
      }
      break;
    }

    case PageId::VirtualMachineId: {
      // Check whether to show PartitionTableWarningPage.
      if (!IsPartitionTableMatch()) {
        this->setCurrentPage(PageId::PartitionTableWarningId);
      } else {
        prev_page_ = current_page_;
        current_page_ = PageId::PartitionTableWarningId;
        this->goNextPage();
      }
      break;
    }

    case PageId::PartitionTableWarningId: {
      // Check whether to show SystemInfoPage.
      if (GetSettingsBool(kSkipSystemInfoPage)) {
        system_info_frame_->writeConf();
        prev_page_ = current_page_;
        current_page_ = PageId::SystemInfoId;
        this->goNextPage();
      } else {
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::SystemInfoId);
      }
      break;
    }

    case PageId::SystemInfoId: {
      // Check whether to show PartitionPage.
      if (GetSettingsBool(kSkipPartitionPage)) {
        if (GetSettingsBool(kPartitionDoAutoPart)) {
          partition_frame_->autoPart();
        }
        prev_page_ = current_page_;
        current_page_ = PageId::PartitionId;
        this->goNextPage();
      } else {
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::PartitionId);
      }
      break;
    }

    case PageId::PartitionId: {
      // Show InstallProgressFrame.
      page_indicator_->goNextPage();
      bool position_animation, opacity_animation;
      GetInstallProgressFrameAnimationLevel(position_animation,
                                            opacity_animation);
      // Set animation level in InstallProgressFrame.
      install_progress_frame_->startSlide(position_animation,
                                          opacity_animation);
      this->setCurrentPage(PageId::InstallProgressId);
      break;
    }

    case PageId::InstallProgressId: {
      // First, save log file to disk.
      if (!SaveLogFileToDisk()) {
        qWarning() << "Failed to save log file to disk!";
      }

      if (install_progress_frame_->failed()) {
        QString msg, encoded_msg;
        if (ReadErrorMsg(msg, encoded_msg)) {
          install_failed_frame_->updateErrorMessage(msg, encoded_msg);
        } else {
          msg = "Error: failed to read log file!";
          encoded_msg = EncodeErrorMsg(msg);
          install_failed_frame_->updateErrorMessage(msg, encoded_msg);
        }
        this->setCurrentPage(PageId::InstallFailedId);
      } else {
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
  // TODO(xushaohua): reboot system
  qApp->quit();
}

void MainWindow::shutdownSystem() {
  // TODO(xushaohua): Poweroff system.
  this->close();
}

}  // namespace installer
