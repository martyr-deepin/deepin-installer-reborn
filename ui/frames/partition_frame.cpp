// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/partition_frame.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedLayout>
#include <QThread>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/frames/delegates/partition_delegate.h"
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

namespace ui {

PartitionFrame::PartitionFrame(QWidget* parent) :
    QFrame(parent),
    partition_delegate_(new PartitionDelegate(this)) {
  this->setObjectName(QStringLiteral("partition_frame"));

  this->initUI();
  this->initConnections();
}

void PartitionFrame::autoPart() {
  partition_delegate_->autoConf();
}

void PartitionFrame::initConnections() {
  connect(simple_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onSimpleFrameButtonToggled);
  connect(advanced_frame_button_, &QPushButton::toggled,
          this, &PartitionFrame::onAdvancedFrameButtonToggled);
  connect(next_button_, &QPushButton::clicked,
          this, &PartitionFrame::onNextButtonClicked);

  connect(partition_delegate_, &PartitionDelegate::deviceRefreshed,
          this, &PartitionFrame::showMainFrame);

  connect(prepare_install_frame_, &PrepareInstallFrame::aborted,
          this, &PartitionFrame::showMainFrame);
  connect(prepare_install_frame_, &PrepareInstallFrame::finished,
          this, &PartitionFrame::finished);
}

void PartitionFrame::initUI() {
  advanced_partition_frame_ = new AdvancedPartitionFrame(partition_delegate_);
  edit_partition_frame_ = new EditPartitionFrame();
  new_partition_frame_ = new NewPartitionFrame();
  partition_loading_frame_ = new PartitionLoadingFrame();
  prepare_install_frame_ = new PrepareInstallFrame();
  select_bootloader_frame_ = new SelectBootloaderFrame();
  simple_partition_frame_ = new SimplePartitionFrame(partition_delegate_);

  TitleLabel* title_label = new TitleLabel(tr("Select Installation Location"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Please make sure you have backed up important data, then select "
         "the disk to install"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  QButtonGroup* button_group = new QButtonGroup(this);
  simple_frame_button_ = new QPushButton(tr("Simple"));
  simple_frame_button_->setObjectName(QStringLiteral("simple_frame_button"));
  simple_frame_button_->setCheckable(true);
  simple_frame_button_->setChecked(true);
  simple_frame_button_->setFlat(true);
  advanced_frame_button_ = new QPushButton(tr("Advanced"));
  advanced_frame_button_->setObjectName(
      QStringLiteral("advanced_frame_button"));
  advanced_frame_button_->setCheckable(true);
  advanced_frame_button_->setFlat(true);
  button_group->addButton(simple_frame_button_);
  button_group->addButton(advanced_frame_button_);
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->setSpacing(0);
  button_layout->setContentsMargins(0, 0, 0, 0);
  button_layout->addStretch();
  button_layout->addWidget(simple_frame_button_);
  button_layout->addWidget(advanced_frame_button_);
  button_layout->addStretch();

  partition_stacked_layout_ = new QStackedLayout();
  partition_stacked_layout_->addWidget(simple_partition_frame_);
  partition_stacked_layout_->addWidget(advanced_partition_frame_);

  next_button_ = new NavButton(tr("Start installation"));
  QHBoxLayout* next_layout = new QHBoxLayout();
  next_layout->addWidget(next_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addLayout(button_layout);
  layout->addLayout(partition_stacked_layout_);
  layout->addStretch();
  layout->addLayout(next_layout);

  main_frame_ = new QFrame();
  main_frame_->setLayout(layout);
  main_frame_->setStyleSheet(
      base::ReadTextFileContent(":/styles/partition_frame_main_frame.css"));

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
  // Show prepare-install-frame page if target partition is selected.
  if (true) {
    main_layout_->setCurrentWidget(prepare_install_frame_);
  }
}

void PartitionFrame::showMainFrame() {
  main_layout_->setCurrentWidget(main_frame_);
}

}  // namespace ui