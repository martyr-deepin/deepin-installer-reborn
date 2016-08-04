// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/main_window.h"

#include <QDebug>
#include <QHash>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QVBoxLayout>

#include "base/gaussian_blur.h"
#include "service/partition_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/virtual_machine.h"
#include "ui/frames/confirm_quit_frame.h"
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

namespace ui {

namespace {

int GetVisiblePages() {
  int pages = 0;
  if (!service::GetSettingsBool(service::kSkipSelectLanguagePageName)) {
    pages += 1;
  }
  if (!service::GetSettingsBool(service::kSkipSystemInfoPageName)) {
    pages += 1;
  }
  if (!service::GetSettingsBool(service::kSkipPartitionPageName)) {
    pages += 1;
  }
  // For install-progress page.
  pages += 1;
  return pages;
}

bool IsDiskSpaceInsufficient() {
  const qint64 maximum_device_size = service::GetMaximumDeviceSize();
  const int required_device_size = service::GetSettingsValue(
      service::kPartitionMinimumDiskSpaceRequiredName).toInt();
  return required_device_size * 1024 * 1024 > maximum_device_size;
}

// Check whether partition table matches machine settings.
bool IsPartitionTableMatch() {
  service::PartitionTableType type = service::GetPrimaryDiskPartitionTable();
  if (type == service::PartitionTableType::Empty) {
    return true;
  }
  if (service::IsEfiEnabled()) {
    return type == service::PartitionTableType::GPT;
  } else {
    return type == service::PartitionTableType::MsDos;
  }
}

}  // namespace

MainWindow::MainWindow()
    : QWidget(),
      pages_(),
      prev_page_(PageId::NullId),
      current_page_(PageId::NullId) {
  this->setObjectName(QStringLiteral("main_window"));

  this->initUI();
  this->initPages();
  this->registerShortcut();
  this->initConnections();

  this->goNextPage();
}

void MainWindow::setCloseButtonVisible(bool visible) {
  close_button_->setVisible(visible);
}

bool MainWindow::isCloseButtonVisible() const {
  return close_button_->isVisible();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  Q_ASSERT(background_label_);
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
  vbox_layout->addSpacing(48);
  vbox_layout->addWidget(page_indicator_wrapper);
  vbox_layout->addSpacing(32);

  this->setLayout(vbox_layout);
  this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::registerShortcut() {
  // TODO(xushaohua): Register Ctrl+L and Ctrl+P actions
}

void MainWindow::setCurrentPage(PageId page_id) {
  Q_ASSERT(pages_.contains(page_id));
  Q_ASSERT(page_id != PageId::NullId);

  prev_page_ = current_page_;
  current_page_ = page_id;
  stacked_layout_->setCurrentIndex(pages_.value(page_id));
}

void MainWindow::updateBackground() {
  const QString image_path = service::GetWindowBackground();
  background_label_->setPixmap(base::FastGaussianBlur(image_path, 32));
  background_label_->setFixedSize(size());
}

void MainWindow::onCloseButtonClicked() {
  this->setCurrentPage(PageId::ConfirmQuitId);
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
      if (sysinfo::IsVirtualMachine()) {
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
      this->setCurrentPage(PageId::InstallProgressId);
      break;
    }

    case PageId::PartitionTableWarningId: {
      // Check whether to show SelectLanguagePage.
      if (!service::GetSettingsBool(service::kSkipSelectLanguagePageName)) {
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
      if (!service::GetSettingsBool(service::kSkipSystemInfoPageName)) {
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
      if (!service::GetSettingsBool(service::kSkipPartitionPageName)) {
        page_indicator_->setVisible(true);
        page_indicator_->goNextPage();
        this->setCurrentPage(PageId::PartitionId);
      } else {
        if (service::GetSettingsBool(service::kPartitionDoAutoPartName)) {
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
      if (IsPartitionTableMatch()) {
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
      if (IsDiskSpaceInsufficient()) {
        page_indicator_->setVisible(false);
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
  this->close();
}

void MainWindow::shutdownSystem() {
  // TODO(xushaohua): Poweroff system.
  this->close();
}

}  // namespace ui
