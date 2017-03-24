// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_ANIMATIONS_MAIN_WINDOW_H
#define INSTALLER_UI_ANIMATIONS_MAIN_WINDOW_H

#include <QFrame>
#include <QVector>
class QGraphicsOpacityEffect;
class QLabel;
class QParallelAnimationGroup;
class QPropertyAnimation;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

namespace installer {

class AnimationsMainWindow : public QFrame {
 Q_OBJECT
 public:
  explicit AnimationsMainWindow(QWidget* parent = nullptr);

 private:
  void initConnections();

  void initUI();

  void showMessage(QLabel* label);
  void hideMessage(QLabel* label);

  QPushButton* show_all_btn_ = nullptr;
  QPushButton* show_one_btn_ = nullptr;
  QPushButton* hide_all_btn_ = nullptr;
  QPushButton* hide_one_btn_ = nullptr;

  QFrame* msg_container_frame_ = nullptr;
  QVBoxLayout* msg_layout_ = nullptr;
  QVector<QLabel*> msg_labels_;

  QFrame* partitions_frame_ = nullptr;
  QScrollArea* scroll_area_ = nullptr;
  QFrame* scroll_frame_ = nullptr;

 private slots:
  void onHideAllBtnClicked();
  void onHideOneBtnClicked();
  void onShowAllBtnClicked();
  void onShowOneBtnClicked();
};

}  // namespace installer

#endif  // INSTALLER_UI_ANIMATIONS_MAIN_WINDOW_H
