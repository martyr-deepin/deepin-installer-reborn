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

#ifndef INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H
#define INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H

#include <QFrame>
#include <QPixmap>
class QHBoxLayout;
class QLabel;

namespace installer {

// PageIndicator is used to display page state.
class PageIndicator : public QFrame {
  Q_OBJECT

 public:
  PageIndicator(int pages, QWidget* parent = nullptr);

  // Switch to next page indicator.
  void goNextPage();
  void setCurrentPage(int page);
  int currentPage() const { return current_page_; };
  void updatePages(int pages);

 private:
  void initUI();
  void updateIndicators();

  int pages_;
  int current_page_;

  // To keep indicator icons.
  QHBoxLayout* layout_ = nullptr;
  QPixmap indicator_active_;
  QPixmap indicator_inactive_;
  QList<QLabel*> m_indicators;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H
