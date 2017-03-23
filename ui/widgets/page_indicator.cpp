// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/page_indicator.h"

#include <QHBoxLayout>
#include <QLabel>

namespace installer {

PageIndicator::PageIndicator(int pages, QWidget* parent)
  : QFrame(parent),
    pages_(pages),
    current_page_(-1),
    indicator_active_(":/images/indicator_active.svg"),
    indicator_inactive_(":/images/indicator_inactive.svg") {
  Q_ASSERT(pages > 0);
  this->setObjectName("page_indicator");

  this->initUI();
  this->updateIndicators();
}

void PageIndicator::goNextPage() {
  Q_ASSERT(current_page_ < pages_);
  this->setCurrentPage(current_page_ + 1);
}

void PageIndicator::setCurrentPage(int page) {
  current_page_ = page;
  this->updateIndicators();
}

void PageIndicator::initUI() {
  layout_ = new QHBoxLayout();
  layout_->setAlignment(Qt::AlignCenter);

  for (int i = 0; i < pages_; i++) {
    QLabel* label = new QLabel();
    label->setFixedSize(13, 13);
    layout_->addWidget(label);
  }

  this->setLayout(layout_);
}

void PageIndicator::updateIndicators() {
  for (int i = 0; i < pages_; i++) {
    QLabel* label = qobject_cast<QLabel*>(layout_->itemAt(i)->widget());
    Q_ASSERT(label);
    if (label != nullptr) {
      if (i == current_page_) {
        label->setPixmap(indicator_active_);
      } else {
        label->setPixmap(indicator_inactive_);
      }
    }
  }
}


}  // namespace installer
