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

#include "application.h"
#include "base/gaussian_blur.h"
#include "service/settings_manager.h"
#include "ui/frames/confirm_quit_frame.h"
#include "ui/frames/disk_space_insufficient_frame.h"
#include "ui/frames/install_failed_frame.h"
#include "ui/frames/install_progress_frame.h"
#include "ui/frames/install_success_frame.h"
#include "ui/frames/partition_table_warning_frame.h"
#include "ui/frames/select_language_frame.h"
#include "ui/frames/system_info_frame.h"
#include "ui/frames/virtual_machine_frame.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/page_indicator.h"

namespace ui {

namespace {

const int kCloseButtonSize = 32;

// Frame page names.
const char kConfirmQuitFrameName[] = "confirm_quit_frame";
const char kDiskSpaceInsufficientFrameName[] = "disk_space_insufficient_frame";
const char kInstallFailedFrameName[] = "install_failed_frame";
const char kInstallProgressFrameName[] = "install_progress_frame";
const char kInstallSuccessFrameName[] = "install_success_frame";
const char kPartitionTableWarningFrameName[] = "partition_table_warning_frame";
const char kSelectLanguageFrameName[] = "select_language_frame";
const char kSystemInfoFrameName[] = "system_info_frame";
const char kVirtualMachineFrameName[] = "virtual_machine_frame";

}  // namespace

MainWindow::MainWindow()
    : QWidget(),
      pages_() {
  this->setObjectName(QStringLiteral("main_window"));

  this->initUI();
  this->initPages();
  this->registerShortcut();
  this->initConnections();

  this->setCurrentPage(kVirtualMachineFrameName);
}

void MainWindow::setCloseButtonVisible(bool visible) {
  close_button_->setVisible(visible);
}

bool MainWindow::isCloseButtonVisible() const {
  return close_button_->isVisible();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  Q_ASSERT(background_label_);
  Q_ASSERT(close_button_);

  this->updateBackground();

  // Move close button to top-right corner of main window.
  // Do not call event->size().width(), which stores an incorrect value.
  close_button_->move(this->width() - kCloseButtonSize - 10, 10);

  QWidget::resizeEvent(event);
}

void MainWindow::initConnections() {
  connect(close_button_, &QPushButton::clicked,
          this, &MainWindow::onCloseButtonClicked);
}

void MainWindow::initPages() {
  ConfirmQuitFrame* confirm_quit_frame = new ConfirmQuitFrame(this);
  pages_.insert(kConfirmQuitFrameName,
                stacked_layout_->addWidget(confirm_quit_frame));

  DiskSpaceInsufficientFrame* disk_space_insufficient_frame =
      new DiskSpaceInsufficientFrame(this);
  pages_.insert(kDiskSpaceInsufficientFrameName,
                stacked_layout_->addWidget(disk_space_insufficient_frame));

  InstallFailedFrame* install_failed_frame = new InstallFailedFrame(this);
  pages_.insert(kInstallFailedFrameName,
                stacked_layout_->addWidget(install_failed_frame));

  InstallProgressFrame* install_progress_frame = new InstallProgressFrame(this);
  pages_.insert(kInstallProgressFrameName,
                stacked_layout_->addWidget(install_progress_frame));

  InstallSuccessFrame* install_success_frame = new InstallSuccessFrame(this);
  pages_.insert(kInstallSuccessFrameName,
                stacked_layout_->addWidget(install_success_frame));

  PartitionTableWarningFrame* partition_table_warning_frame =
      new PartitionTableWarningFrame(this);
  pages_.insert(kPartitionTableWarningFrameName,
                stacked_layout_->addWidget(partition_table_warning_frame));

  SelectLanguageFrame* select_language_frame = new SelectLanguageFrame(this);
  pages_.insert(kSelectLanguageFrameName,
                stacked_layout_->addWidget(select_language_frame));

  SystemInfoFrame* system_info_frame = new SystemInfoFrame(this);
  pages_.insert(kSystemInfoFrameName,
                stacked_layout_->addWidget(system_info_frame));

  VirtualMachineFrame* virtual_machine_frame = new VirtualMachineFrame(this);
  pages_.insert(kVirtualMachineFrameName,
                stacked_layout_->addWidget(virtual_machine_frame));
}

void MainWindow::initUI() {
  background_label_ = new QLabel(this);

  close_button_ = new IconButton(QStringLiteral(":/images/close_normal.png"),
                                 QStringLiteral(":/images/close_hover.png"),
                                 QStringLiteral(":/images/close_press.png"),
                                 kCloseButtonSize,
                                 kCloseButtonSize,
                                 nullptr);
  QHBoxLayout* top_layout = new QHBoxLayout();
  top_layout->setSpacing(0);
  top_layout->setContentsMargins(0, 0, 0, 0);
  top_layout->setAlignment(Qt::AlignRight);
  top_layout->addWidget(close_button_);

  stacked_layout_ = new QStackedLayout();

  // Use a wrapper to hold its position.
  QFrame* page_indicator_wrapper = new QFrame();
  page_indicator_wrapper->setFixedHeight(48);
  page_indicator_ = new PageIndicator(visible_pages_, page_indicator_wrapper);
  QHBoxLayout* indicator_layout = new QHBoxLayout();
  indicator_layout->addWidget(page_indicator_);
  page_indicator_wrapper->setLayout(indicator_layout);

  QVBoxLayout* vbox_layout = new QVBoxLayout();
  vbox_layout->setContentsMargins(0, 0, 0, 0);
  vbox_layout->setSpacing(0);
  vbox_layout->addLayout(top_layout);
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

void MainWindow::setCurrentPage(const QString& frame_name) {
  Q_ASSERT(pages_.contains(frame_name));
  stacked_layout_->setCurrentIndex(pages_.value(frame_name));
}

void MainWindow::updateBackground() {
  const QString image_path = app->settings_manager->getWindowBackground();
  background_label_->setPixmap(base::FastGaussianBlur(image_path, 32));
  background_label_->setFixedSize(size());
}

void MainWindow::onCloseButtonClicked() {
  stacked_layout_->setCurrentIndex(pages_.value(kConfirmQuitFrameName));
}

}  // namespace ui
