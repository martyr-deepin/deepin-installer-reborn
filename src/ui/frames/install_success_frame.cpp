/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui/frames/install_success_frame.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QtCore/QEvent>

#include "ui/widgets/comment_label.h"
#include "ui/widgets/nav_button.h"
#include "ui/widgets/title_label.h"
#include "ui/frames/consts.h"
#include "ui/utils/widget_util.h"

namespace installer {

InstallSuccessFrame::InstallSuccessFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("install_success_frame");

  this->initUI();
  this->initConnections();
}

void InstallSuccessFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    title_label_->setText(tr("Successfully Installed"));
    comment_label_->setText(
        tr("Reboot to enjoy the new experience with deepin, "
           "hope you like it!"));
    eject_label_->setText(
        tr("Please remove the installation media before reboot"));
    reboot_button_->setText(tr("Experience now"));
  } else {
    QFrame::changeEvent(event);
  }
}

void InstallSuccessFrame::initConnections() {
  connect(reboot_button_, &QPushButton::clicked,
          this, &InstallSuccessFrame::finished);
}

void InstallSuccessFrame::initUI() {
  QLabel* status_label = new QLabel();
  status_label->setPixmap(installer::renderPixmap(":/images/success.svg"));
  title_label_ = new TitleLabel(tr("Successfully Installed"));
  comment_label_ = new CommentLabel(
      tr("Reboot to enjoy the new experience with deepin, hope you like it!"));
  QHBoxLayout* comment_layout = new QHBoxLayout();
  comment_layout->setContentsMargins(0, 0, 0, 0);
  comment_layout->setSpacing(0);
  comment_layout->addWidget(comment_label_);

  eject_label_ = new CommentLabel(
      tr("Please remove the installation media before reboot"));
  reboot_button_ = new NavButton(tr("Experience now"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(status_label, 0, Qt::AlignCenter);
  layout->addWidget(title_label_, 0, Qt::AlignCenter);
  layout->addLayout(comment_layout);
  layout->addStretch();
  layout->addWidget(eject_label_, 0, Qt::AlignCenter);
  layout->addWidget(reboot_button_, 0, Qt::AlignCenter);

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

void InstallSuccessFrame::setEjectLabelVisible(bool visible) {
  eject_label_->setVisible(visible);
}

}  // namespace installer
