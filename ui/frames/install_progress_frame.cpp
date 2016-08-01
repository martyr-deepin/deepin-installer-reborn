// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_progress_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QThread>
#include <QVBoxLayout>

#include "service/hooks_manager.h"
#include "service/signal_manager.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/title_label.h"

namespace ui {

InstallProgressFrame::InstallProgressFrame(QWidget* parent)
    : QFrame(parent),
      failed_(false),
      hooks_manager_(new service::HooksManager()),
      hooks_manager_thread_(new QThread()) {
  this->setObjectName(QStringLiteral("install_progress_frame"));

  hooks_manager_thread_->start();
  hooks_manager_->moveToThread(hooks_manager_thread_);

  this->initUI();
  this->initConnections();
}

InstallProgressFrame::~InstallProgressFrame() {
  delete hooks_manager_;
  hooks_manager_ = nullptr;
}

void InstallProgressFrame::initConnections() {
  connect(hooks_manager_, &service::HooksManager::errorOccurred,
          this, &InstallProgressFrame::onErrorOccurred);
  connect(hooks_manager_, &service::HooksManager::processUpdate,
          this, &InstallProgressFrame::onInstallProgressUpdated);
  connect(hooks_manager_, &service::HooksManager::finished,
          this, &InstallProgressFrame::finished);

  service::SignalManager* signal_manager = service::SignalManager::instance();
  connect(signal_manager, &service::SignalManager::autoPartDone,
          this, &InstallProgressFrame::onPartitionDone);
  connect(signal_manager, &service::SignalManager::manualPartDone,
          this, &InstallProgressFrame::onPartitionDone);
}

void InstallProgressFrame::initUI() {
  TitleLabel* title_label = new TitleLabel(tr("Installing"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("You will be experiencing the incredible pleasant of deepin after "
         "the time for just a cup of coffee"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();

  this->setLayout(layout);
}

void InstallProgressFrame::onErrorOccurred() {
  failed_ = true;
  emit this->finished();
}

void InstallProgressFrame::onInstallProgressUpdated(int progress) {
  Q_UNUSED(progress);
}

void InstallProgressFrame::onPartitionDone(bool ok) {
  if (ok) {
    onInstallProgressUpdated(5);
    emit hooks_manager_->runHooks();
  } else {
    this->onErrorOccurred();
  }
}

}  // namespace ui