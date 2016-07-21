// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/page_indicator.h"

#include <QHBoxLayout>
#include <QLabel>

namespace ui {

PageIndicator::PageIndicator(int pages, QWidget* parent)
  : QFrame(parent),
    pages_(pages),
    current_page_(-1),
    indicator_active_(QStringLiteral(":/images/indicator-active.png")),
    indicator_inactive_(QStringLiteral(":/images/indicator-inactive.png")) {
  Q_ASSERT(pages > 0);
  this->setObjectName(QStringLiteral("page_indicator"));

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

  for (int i = 0; i < pages_; i++) {
    QLabel* label = new QLabel();
    layout_->addWidget(label);
  }

  layout_->setAlignment(Qt::AlignCenter);
  this->setLayout(layout_);
}

void PageIndicator::updateIndicators() {
  for (int i = 0; i < pages_; i++) {
    QLabel* label = static_cast<QLabel*>(layout_->itemAt(i)->widget());
    if (i == current_page_) {
      label->setPixmap(indicator_active_);
    } else {
      label->setPixmap(indicator_inactive_);
    }
  }
}


}  // namespace ui
