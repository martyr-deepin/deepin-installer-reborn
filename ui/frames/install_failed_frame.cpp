// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_failed_frame.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "service/log_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/frosted_frame.h"
#include "ui/widgets/icon_button.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/qr_widget.h"
#include "ui/widgets/title_label.h"

namespace ui {

namespace {

const int kContentWindowWidth = 640;
const int kContentWindowHeight = 480;
const int kQRWindowSize = 320;
const int kControlButtonSize = 32;

const int kLabelContentStripped = 320;
const int kQRContentStripped = 80;

}

InstallFailedFrame::InstallFailedFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("install_failed_frame"));

  this->initUI();
  qr_widget_->setVisible(false);
  this->initConnections();
}

void InstallFailedFrame::updateErrorMessage() {
  // Read from log file.
  const QString content = base::ReadTextFileContent(service::GetLogFilepath());
  content_label_->setText(content.right(kLabelContentStripped));

  // Encode with base64.
  const QString base64_content =
      content.right(kQRContentStripped).toUtf8().toBase64();
  const QString prefix =
      service::GetSettingsString(service::kInstallFailedFeedbackServerName);
  qr_widget_->setText(prefix.arg(base64_content));
}

void InstallFailedFrame::initConnections() {
  connect(control_button_, &QPushButton::clicked,
          this, &InstallFailedFrame::onControlButtonClicked);
  connect(reboot_button_, &QPushButton::clicked,
          this, &InstallFailedFrame::finished);
}

void InstallFailedFrame::initUI() {
  QLabel* status_label = new QLabel();
  status_label->setPixmap(QPixmap(":/images/failed.png"));
  QHBoxLayout* status_layout = new QHBoxLayout();
  status_layout->setAlignment(Qt::AlignCenter);
  status_layout->addWidget(status_label);

  TitleLabel* title_label = new TitleLabel(tr("Installation Failed"));
  QHBoxLayout* title_layout = new QHBoxLayout();
  title_layout->addWidget(title_label);

  CommentLabel* comment_label = new CommentLabel(
      tr("Sorry for the inconvenience, you can photo or scan the 2D code "
         "to send us the error log, so we can better solve the issue."));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->addWidget(comment_label);

  FrostedFrame* content_frame = new FrostedFrame();
  content_frame->setFixedSize(kContentWindowWidth, kContentWindowHeight);
  QHBoxLayout* content_layout = new QHBoxLayout();
  content_layout->setAlignment(Qt::AlignCenter);
  content_layout->addWidget(content_frame);
  content_label_ = new QLabel(content_frame);
  content_label_->setFixedSize(kContentWindowWidth, kContentWindowHeight);
  content_label_->setWordWrap(true);
  content_label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  content_label_->setStyleSheet(
      base::ReadTextFileContent(":/styles/install_failed_label.css"));
  qr_widget_ = new QRWidget(content_frame);
  qr_widget_->setFixedSize(kQRWindowSize, kQRWindowSize);
  qr_widget_->move((kContentWindowWidth - kQRWindowSize) / 2,
                   (kContentWindowHeight - kQRWindowSize) / 2);
  control_button_ = new IconButton(QStringLiteral(":/images/QR_normal.png"),
                                   QStringLiteral(":/images/QR_hover.png"),
                                   QStringLiteral(":/images/QR_press.png"),
                                   kControlButtonSize, kControlButtonSize,
                                   content_frame);
  control_button_->move(kContentWindowWidth - kControlButtonSize, 0);

  reboot_button_ = new NavButton(tr("Exit installation"));
  QHBoxLayout* reboot_layout = new QHBoxLayout();
  reboot_layout->addWidget(reboot_button_);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addLayout(status_layout);
  layout->addLayout(title_layout);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addLayout(content_layout);
  layout->addStretch();
  layout->addLayout(reboot_layout);

  this->setLayout(layout);
}

void InstallFailedFrame::onControlButtonClicked() {
  if (content_label_->isVisible()) {
    content_label_->setVisible(false);
    qr_widget_->setVisible(true);
  } else {
    content_label_->setVisible(true);
    qr_widget_->setVisible(false);
  }
}

}  // namespace ui