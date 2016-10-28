// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/inner/partition_loading_frame.h"

#include <QHBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/title_label.h"

namespace installer {

PartitionLoadingFrame::PartitionLoadingFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("partition_loading_frame"));

  this->initUI();
  this->initConnections();
}

void PartitionLoadingFrame::initConnections() {

}

void PartitionLoadingFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Scanning disks.."));
  CommentLabel* comment_label = new CommentLabel(
      tr("It may take a few seconds to scan disk devices..."));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(title_label, 0, Qt::AlignCenter);
  layout->addWidget(comment_label, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
}

}  // namespace installer