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

#include "ui/widgets/page_indicator.h"
#include "ui/utils/widget_util.h"

#include <QHBoxLayout>
#include <QLabel>

namespace installer {

PageIndicator::PageIndicator(int pages, QWidget* parent)
  : QFrame(parent),
    pages_(pages),
    current_page_(-1),
    indicator_active_(installer::renderPixmap(":/images/indicator_active.svg")),
    indicator_inactive_(installer::renderPixmap(":/images/indicator_inactive.svg")) {
  Q_ASSERT(pages > 0);
  this->setObjectName("page_indicator");

  this->initUI();
}

void PageIndicator::goNextPage() {
  Q_ASSERT(current_page_ < pages_);
  this->setCurrentPage(current_page_ + 1);
}

void PageIndicator::setCurrentPage(int page) {
  current_page_ = page;
  this->updateIndicators();
}

void PageIndicator::updatePages(int pages)
{
    pages_ = pages;

    for (QLabel *w : m_indicators) {
        layout_->removeWidget(w);
        w->deleteLater();
    }

    m_indicators.clear();

    for (int i = 0; i != pages; ++i) {
        QLabel* label = new QLabel;
        label->setFixedSize(13, 13);
        label->setPixmap(indicator_inactive_);
        layout_->addWidget(label);
        m_indicators << label;
    }

    updateIndicators();
}

void PageIndicator::initUI() {
  layout_ = new QHBoxLayout();
  layout_->setAlignment(Qt::AlignCenter);

  updatePages(pages_);

  this->setLayout(layout_);
}

void PageIndicator::updateIndicators() {
    for (int i = 0; i < pages_; i++) {
        QLabel* label = m_indicators[i];
        label->setPixmap(i == current_page_ ? indicator_active_ : indicator_inactive_);
    }
}

}  // namespace installer
