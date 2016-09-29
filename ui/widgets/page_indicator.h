// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H
#define INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H

#include <QFrame>
#include <QPixmap>
class QHBoxLayout;

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

 private:
  void initUI();
  void updateIndicators();

  int pages_;
  int current_page_;

  // To keep indicator icons.
  QHBoxLayout* layout_ = nullptr;
  QPixmap indicator_active_;
  QPixmap indicator_inactive_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_PAGE_INDICATOR_H
