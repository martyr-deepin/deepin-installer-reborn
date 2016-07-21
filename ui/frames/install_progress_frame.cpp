// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_progress_frame.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "service/hooks_manager.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/title_label.h"

namespace ui {

InstallProgressFrame::InstallProgressFrame(QWidget* parent)
    : QFrame(parent),
      failed_(false),
      error_message_(),
      hooks_inited_(false) {
  this->setObjectName(QStringLiteral("install_progress_frame"));

  hooks_manager_ = new service::HooksManager();
  // hooks_manager_->moveToThread();

  this->initUI();
  this->initConnections();
}

InstallProgressFrame::~InstallProgressFrame() {
  delete hooks_manager_;
  hooks_manager_ = nullptr;
}

void InstallProgressFrame::initHooks() {
  qDebug() << "init hooks()";
  if (!hooks_inited_) {
    hooks_inited_ = true;
    emit hooks_manager_->runHooks();
  }
}

void InstallProgressFrame::initConnections() {
  connect(hooks_manager_, &service::HooksManager::errorOccurred,
          this, &InstallProgressFrame::onErrorOccurred);
  connect(hooks_manager_, &service::HooksManager::processUpdate,
          this, &InstallProgressFrame::onInstallProgressUpdated);
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

void InstallProgressFrame::onErrorOccurred(const QString& msg) {
  qDebug() << "[InstallProgressFrame]::onErrorOccurred()" << msg;
  failed_ = true;
  error_message_ = msg;
  emit this->finished();
}

void InstallProgressFrame::onInstallProgressUpdated(int progress) {

}

}  // namespace ui