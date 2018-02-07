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

#include "ui/frames/inner/new_table_loading_frame.h"

#include <QEvent>
#include <QVBoxLayout>

#include "ui/frames/consts.h"
#include "ui/widgets/comment_label.h"
#include "ui/widgets/spinner_label.h"

namespace installer {

NewTableLoadingFrame::NewTableLoadingFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("new_table_loading_frame");

  this->initUI();
}

void NewTableLoadingFrame::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    comment_label_->setText(tr("Formatting..."));
  } else {
    QFrame::changeEvent(event);
  }
}

void NewTableLoadingFrame::hideEvent(QHideEvent* event) {
  spinner_label_->stop();
  QFrame::hideEvent(event);
}

void NewTableLoadingFrame::showEvent(QShowEvent* event) {
  spinner_label_->start();
  QFrame::showEvent(event);
}

void NewTableLoadingFrame::initUI() {
  comment_label_ = new CommentLabel(tr("Formatting..."));
  spinner_label_ = new SpinnerLabel();

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(kMainLayoutSpacing);
  layout->addStretch();
  layout->addWidget(spinner_label_, 0, Qt::AlignCenter);
  layout->addSpacing(15);
  layout->addWidget(comment_label_, 0, Qt::AlignCenter);
  layout->addStretch();

  this->setLayout(layout);
  this->setContentsMargins(0, 0, 0, 0);
}

}  // namespace installer