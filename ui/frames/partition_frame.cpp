// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_frame.h"

#include <QButtonGroup>
#include <QEvent>
#include <QHBoxLayout>
#include <QStackedLayout>

#include "base/file_util.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/frames/inner/advanced_partition_frame.h"
#include "ui/frames/inner/edit_partition_frame.h"
#include "ui/frames/inner/new_partition_frame.h"
#include "ui/frames/inner/partition_loading_frame.h"
#include "ui/frames/inner/prepare_install_frame.h"
#include "ui/frames/inner/select_bootloader_frame.h"
#include "ui/frames/inner/simple_partition_frame.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"

namespace installer {

PartitionFrame::PartitionFrame(QWidget* parent)
    : QFrame(parent),
      delegate_(new PartitionDelegate(this)) {
  this->setObjectName("partition_frame");

  this->initUI();
  this->initConnections();
}

void PartitionFrame::autoPart() {
  delegate_->autoConf();
}

void PartitionFrame::scanDevices() const {
  delegate_->scanDevices();
}

void PartitionFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Select Installation Location"));
    comment_label_->setText(
        tr("Please make sure you have backed up "
           "important data, then select the disk to install"));
    simple_frame_button_->setText(tr("Simple"));
    advanced_frame_button_->setText(tr("Advanced"));
    next_button_->setText(tr("Start installation"));
  } else {
    QFrame::changeEvent(event);
  }
}

void PartitionFrame::initConnections() {
  connect(simple_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onSimpleFrameButtonToggled);
  connect(advanced_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onAdvancedFrameButtonToggled);
  connect(next_button_, &QPushButton::clicked,
          this, &PartitionFrame::onNextButtonClicked);

  // Show main frame when device is refreshed.
  connect(delegate_, &PartitionDelegate::deviceRefreshed,
          this, &PartitionFrame::showMainFrame);
  connect(delegate_, &PartitionDelegate::autoPartDone,
          this, &PartitionFrame::autoPartDone);
  connect(delegate_, &PartitionDelegate::manualPartDone,
          this, &PartitionFrame::manualPartDone);

  connect(advanced_partition_frame_,
          &AdvancedPartitionFrame::requestEditPartitionFrame,
          this, &PartitionFrame::showEditPartitionFrame);
  connect(advanced_partition_frame_,
          &AdvancedPartitionFrame::requestNewPartitionFrame,
          this, &PartitionFrame::showNewPartitionFrame);
  connect(advanced_partition_frame_,
          &AdvancedPartitionFrame::requestSelectBootloaderFrame,
          this, &PartitionFrame::showSelectBootloaderFrame);

  connect(edit_partition_frame_, &EditPartitionFrame::finished,
          this, &PartitionFrame::showMainFrame);
  connect(new_partition_frame_, &NewPartitionFrame::finished,
          this, &PartitionFrame::showMainFrame);

  connect(prepare_install_frame_, &PrepareInstallFrame::aborted,
          this, &PartitionFrame::showMainFrame);
  connect(prepare_install_frame_, &PrepareInstallFrame::finished,
          this, &PartitionFrame::onPrepareInstallFrameFinished);

  connect(select_bootloader_frame_, &SelectBootloaderFrame::bootloaderUpdated,
          advanced_partition_frame_,
          &AdvancedPartitionFrame::setBootloaderPath);
  connect(select_bootloader_frame_, &SelectBootloaderFrame::bootloaderUpdated,
          delegate_, &PartitionDelegate::setBootloaderPath);
  connect(select_bootloader_frame_, &SelectBootloaderFrame::finished,
          this, &PartitionFrame::showMainFrame);
}

void PartitionFrame::initUI() {
  advanced_partition_frame_ = new AdvancedPartitionFrame(delegate_, this);
  edit_partition_frame_ = new EditPartitionFrame(delegate_, this);
  new_partition_frame_ = new NewPartitionFrame(delegate_, this);
  partition_loading_frame_ = new PartitionLoadingFrame(this);
  prepare_install_frame_ = new PrepareInstallFrame(delegate_, this);
  select_bootloader_frame_ = new SelectBootloaderFrame(delegate_, this);
  simple_partition_frame_ = new SimplePartitionFrame(delegate_, this);

  title_label_ = new TitleLabel(tr("Select Installation Location"));
  comment_label_ = new CommentLabel(
      tr("Please make sure you have backed up "
         "important data, then select the disk to install"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  QButtonGroup* button_group = new QButtonGroup(this);
  simple_frame_button_ = new PointerButton(tr("Simple"));
  simple_frame_button_->setObjectName("simple_frame_button");
  simple_frame_button_->setCheckable(true);
  simple_frame_button_->setChecked(true);
  simple_frame_button_->setFlat(true);
  simple_frame_button_->setMinimumWidth(86);
  advanced_frame_button_ = new PointerButton(tr("Advanced"));
  advanced_frame_button_->setObjectName("advanced_frame_button");
  advanced_frame_button_->setCheckable(true);
  advanced_frame_button_->setMinimumWidth(86);
  advanced_frame_button_->setFlat(true);
  button_group->addButton(simple_frame_button_);
  button_group->addButton(advanced_frame_button_);
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->setContentsMargins(0, 0, 0, 0);
  button_layout->setSpacing(0);
  button_layout->addStretch();
  button_layout->addWidget(simple_frame_button_, 0, Qt::AlignRight);
  button_layout->addWidget(advanced_frame_button_, 0, Qt::AlignLeft);
  button_layout->addStretch();

  partition_stacked_layout_ = new QStackedLayout();
  partition_stacked_layout_->setContentsMargins(0, 0, 0, 0);
  partition_stacked_layout_->setSpacing(0);
  if (GetSettingsBool(kPartitionSkipSimplePartitionPage)) {
    // Hide simple partition frame if it is disabled.
    simple_partition_frame_->hide();
  } else {
    partition_stacked_layout_->addWidget(simple_partition_frame_);
  }
  if (GetSettingsBool(kPartitionSkipAdvancedPartitionPage)) {
    // Hide advanced partition frame if it is disabled.
    advanced_partition_frame_->hide();
  } else {
    partition_stacked_layout_->addWidget(advanced_partition_frame_);
  }

  // TODO(xushaohua): Remove this layout.
  QHBoxLayout* partition_stacked_wrapper_layout = new QHBoxLayout();
  partition_stacked_wrapper_layout->setContentsMargins(0, 0, 0, 0);
  partition_stacked_wrapper_layout->setSpacing(0);
  partition_stacked_wrapper_layout->addStretch();
  partition_stacked_wrapper_layout->addLayout(partition_stacked_layout_);
  partition_stacked_wrapper_layout->addStretch();

  // TODO(xushaohua): Move this button to simple partition
  // and advanced partition page.
  next_button_ = new NavButton(tr("Start installation"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->setContentsMargins(0, 0, 0, 0);
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(title_label_, 0, Qt::AlignHCenter);
  layout->addSpacing(kMainLayoutSpacing);
  layout->addLayout(comment_layout);
  layout->addSpacing(kMainLayoutSpacing);
  if (!GetSettingsBool(kPartitionSkipSimplePartitionPage) &&
      !GetSettingsBool(kPartitionSkipAdvancedPartitionPage)) {
    // Add control button groups only if both simple mode and advanced mode
    // are enabled.
    layout->addLayout(button_layout);
  }
  layout->addSpacing(20 + kMainLayoutSpacing);
  layout->addLayout(partition_stacked_wrapper_layout);
  layout->addLayout(next_layout);

  main_frame_ = new QFrame();
  main_frame_->setContentsMargins(0, 0, 0, 0);
  main_frame_->setLayout(layout);
  main_frame_->setStyleSheet(ReadFile(":/styles/partition_main_frame.css"));

  main_layout_ = new QStackedLayout();
  main_layout_->setContentsMargins(0, 0, 0, 0);
  main_layout_->addWidget(partition_loading_frame_);
  main_layout_->addWidget(main_frame_);
  main_layout_->addWidget(edit_partition_frame_);
  main_layout_->addWidget(new_partition_frame_);
  main_layout_->addWidget(prepare_install_frame_);
  main_layout_->addWidget(select_bootloader_frame_);

  this->setLayout(main_layout_);
  this->setContentsMargins(0, 0, 0, 0);
}

void PartitionFrame::onSimpleFrameButtonToggled() {
  partition_stacked_layout_->setCurrentWidget(simple_partition_frame_);
}

void PartitionFrame::onAdvancedFrameButtonToggled() {
  // Refresh device list before showing advanced partition frame.
  // Because mount-point of partitions might have be updated.
  delegate_->refreshVisual();
  partition_stacked_layout_->setCurrentWidget(advanced_partition_frame_);
}

void PartitionFrame::onNextButtonClicked() {
  QWidget* current_widget = partition_stacked_layout_->currentWidget();
  const bool is_simple_page = (current_widget == simple_partition_frame_);
  if (is_simple_page) {
    // Validate simple partition frame.
    if (!simple_partition_frame_->validate()) {
      return;
    }
  } else {
    // Validate advanced partition frame.
    if (!advanced_partition_frame_->validate()) {
      return;
    }
  }

  prepare_install_frame_->updateDescription();
  main_layout_->setCurrentWidget(prepare_install_frame_);
}

void PartitionFrame::onPrepareInstallFrameFinished() {
  QWidget* current_widget = partition_stacked_layout_->currentWidget();
  const bool is_simple_page = (current_widget == simple_partition_frame_);
  delegate_->doManualPart(is_simple_page);

  emit this->finished();
}

void PartitionFrame::showEditPartitionFrame(const Partition& partition) {
  edit_partition_frame_->setPartition(partition);
  main_layout_->setCurrentWidget(edit_partition_frame_);
}

void PartitionFrame::showMainFrame() {
  main_layout_->setCurrentWidget(main_frame_);
}

void PartitionFrame::showNewPartitionFrame(
    const Partition& partition) {
  new_partition_frame_->setPartition(partition);
  main_layout_->setCurrentWidget(new_partition_frame_);
}

void PartitionFrame::showSelectBootloaderFrame() {
  main_layout_->setCurrentWidget(select_bootloader_frame_);
}

}  // namespace installer
