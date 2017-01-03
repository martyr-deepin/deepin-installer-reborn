// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/install_failed_frame.h"

#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/frosted_frame.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/qr_widget.h"
#include "ui/widgets/title_label.h"

namespace installer {

namespace {

const int kContentWindowWidth = 580;
const int kContentWindowHeight = 360;

const int kQrMargin = 8;
const int kQrWindowSize = 300;

const int kControlButtonSize = 32;

}  // namespace

InstallFailedFrame::InstallFailedFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("install_failed_frame");

  this->initUI();
  this->initConnections();

  // Show QR widget by default.
  content_label_->hide();
}

void InstallFailedFrame::updateErrorMessage(const QString& msg,
                                            const QString& encoded_msg) {
  content_label_->setText(msg);
  if (!encoded_msg.isEmpty()) {
    // If encoded_msg if empty, qr_widget will generate a rectangle filled with
    // red color, which is not what we expect.
    qr_widget_->setText(encoded_msg);
  }
}

void InstallFailedFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Installation Failed"));
    comment_label_->setText(
        tr("Sorry for the inconvenience, you can photo or "
           "scan the 2D code to send us the error log, so we can better "
            "solve the issue."));
    reboot_button_->setText(tr("Exit installation"));
  } else {
    QFrame::changeEvent(event);
  }
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
  title_label_ = new TitleLabel(tr("Installation Failed"));
  comment_label_ = new CommentLabel(
      tr("Sorry for the inconvenience, you can photo or "
         "scan the 2D code to send us the error log, so we can better "
         "solve the issue."));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  FrostedFrame* content_frame = new FrostedFrame();
  content_frame->setFixedSize(kContentWindowWidth, kContentWindowHeight);
  content_label_ = new QLabel(content_frame);
  content_label_->setObjectName("content_label");
  content_label_->setFixedSize(kContentWindowWidth, kContentWindowHeight);
  content_label_->setWordWrap(true);
  content_label_->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  qr_widget_ = new QRWidget(content_frame);
  qr_widget_->setMargin(kQrMargin);
  qr_widget_->setFixedSize(kQrWindowSize, kQrWindowSize);
  qr_widget_->move((kContentWindowWidth - kQrWindowSize) / 2,
                   (kContentWindowHeight - kQrWindowSize) / 2);

  control_button_ = new PointerButton(content_frame);
  control_button_->setObjectName("control_button");
  control_button_->setFlat(true);
  control_button_->setFixedSize(kControlButtonSize, kControlButtonSize);
  // Move control_button_ to top-right corner of content area.
  control_button_->move(kContentWindowWidth - kControlButtonSize, 0);

  reboot_button_ = new NavButton(tr("Exit installation"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(status_label, 0, Qt::AlignCenter);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(content_frame, 0, Qt::AlignCenter);
  layout->addStretch();
  layout->addWidget(reboot_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setStyleSheet(ReadFile(":/styles/install_failed_frame.css"));
}

void InstallFailedFrame::onControlButtonClicked() {
  // Toggle visibility of content_label and qr_widget_.
  if (content_label_->isVisible()) {
    content_label_->setVisible(false);
    qr_widget_->setVisible(true);
  } else {
    content_label_->setVisible(true);
    qr_widget_->setVisible(false);
  }
}

}  // namespace installer