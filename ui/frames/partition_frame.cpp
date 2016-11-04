// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_frame.h"

#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QThread>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/frames/inner/advanced_partition_frame.h"
#include "ui/frames/inner/edit_partition_frame.h"
#include "ui/frames/inner/new_partition_frame.h"
#include "ui/frames/inner/partition_loading_frame.h"
#include "ui/frames/inner/prepare_install_frame.h"
#include "ui/frames/inner/select_bootloader_frame.h"
#include "ui/frames/inner/simple_partition_frame.h"
#include "ui/widgets/comment_label_layout.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/pointer_button.h"
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

void PartitionFrame::initConnections() {
  connect(simple_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onSimpleFrameButtonToggled);
  connect(advanced_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onAdvancedFrameButtonToggled);
  connect(next_button_, &QPushButton::clicked,
          this, &PartitionFrame::onNextButtonClicked);

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
          this, &PartitionFrame::finished);
  connect(prepare_install_frame_, &PrepareInstallFrame::finished,
          delegate_, &PartitionDelegate::doManualPart);

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

  TitleLabel* title_label = new TitleLabel(tr("Select Installation Location"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabelLayout* comment_layout = new CommentLabelLayout(
      tr("Please make sure you have backed up important data, then select "
         "the disk to install"));

  QButtonGroup* button_group = new QButtonGroup(this);
  simple_frame_button_ = new PointerButton(tr("Simple"));
  simple_frame_button_->setObjectName("simple_frame_button");
  simple_frame_button_->setCheckable(true);
  simple_frame_button_->setChecked(true);
  simple_frame_button_->setFlat(true);
  // NOTE(xushaohua): Set size policy
  simple_frame_button_->setMinimumWidth(86);
  advanced_frame_button_ = new PointerButton(tr("Advanced"));
  advanced_frame_button_->setObjectName("advanced_frame_button");
  advanced_frame_button_->setCheckable(true);
  advanced_frame_button_->setMinimumWidth(86);
  advanced_frame_button_->setFlat(true);
  button_group->addButton(simple_frame_button_);
  button_group->addButton(advanced_frame_button_);
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->setSpacing(0);
  button_layout->setContentsMargins(0, 0, 0, 0);
  button_layout->addStretch();
  button_layout->addWidget(simple_frame_button_, 0, Qt::AlignRight);
  button_layout->addWidget(advanced_frame_button_, 0, Qt::AlignLeft);
  button_layout->addStretch();

  partition_stacked_layout_ = new QStackedLayout();
  partition_stacked_layout_->addWidget(simple_partition_frame_);
  partition_stacked_layout_->addWidget(advanced_partition_frame_);
  QHBoxLayout* partition_stacked_wrapper_layout = new QHBoxLayout();
  partition_stacked_wrapper_layout->addStretch();
  partition_stacked_wrapper_layout->addLayout(partition_stacked_layout_);
  partition_stacked_wrapper_layout->addStretch();

  next_button_ = new NavButton(tr("Start installation"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addLayout(button_layout);
  layout->addSpacing(20);
  layout->addLayout(partition_stacked_wrapper_layout);
  layout->addSpacing(20);
  layout->addLayout(next_layout);

  main_frame_ = new QFrame();
  main_frame_->setContentsMargins(0, 0, 0, 0);
  main_frame_->setLayout(layout);
  main_frame_->setStyleSheet(ReadFile(":/styles/partition_main_frame.css"));

  main_layout_ = new QStackedLayout();
  main_layout_->addWidget(partition_loading_frame_);
  main_layout_->addWidget(main_frame_);
  main_layout_->addWidget(edit_partition_frame_);
  main_layout_->addWidget(new_partition_frame_);
  main_layout_->addWidget(prepare_install_frame_);
  main_layout_->addWidget(select_bootloader_frame_);

  this->setLayout(main_layout_);
}

void PartitionFrame::onSimpleFrameButtonToggled() {
  partition_stacked_layout_->setCurrentWidget(simple_partition_frame_);
}

void PartitionFrame::onAdvancedFrameButtonToggled() {
  partition_stacked_layout_->setCurrentWidget(advanced_partition_frame_);
}

void PartitionFrame::onNextButtonClicked() {
  // TODO(xushaohua):Show prepare-install-frame page if target partition is selected.
  if (true) {
    prepare_install_frame_->updateDescription();
    main_layout_->setCurrentWidget(prepare_install_frame_);
  }
}

void PartitionFrame::showEditPartitionFrame(const Partition& partition) {
  edit_partition_frame_->setPartition(partition);
  main_layout_->setCurrentWidget(edit_partition_frame_);
}

void PartitionFrame::showMainFrame() {
  // Hide loading page first.
  partition_loading_frame_->hide();
  main_layout_->setCurrentWidget(main_frame_);
}

void PartitionFrame::showNewPartitionFrame(
    const Partition& partition) {
  new_partition_frame_->setPartition(partition);
  main_layout_->setCurrentWidget(new_partition_frame_);
}

void PartitionFrame::showSelectBootloaderFrame() {
  // Notify SelectBootloaderFrame to repaint.
  select_bootloader_frame_->updatePartitionList();
  main_layout_->setCurrentWidget(select_bootloader_frame_);
}

}  // namespace installer
