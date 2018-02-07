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

#ifndef INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H
#define INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H

#include <QFrame>
#include <QStringList>

class QLabel;
class QGraphicsOpacityEffect;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QVariantAnimation;

namespace installer {

// Displays slide in InstallProgressFrame
class InstallProgressSlideFrame : public QFrame {
  Q_OBJECT

 public:
  explicit InstallProgressSlideFrame(QWidget* parent = nullptr);

 public slots:
  // Start slide show.
  // If |disable_slide| is true, do no switch slide images.
  // If |disable_animation| is true, do no apply animation.
  // |duration| defines animation interval.
  void startSlide(bool disable_slide, bool disable_animation, int duration);

  // Stop slide show.
  void stopSlide();

 private:
  void initConnections();
  void initUI();
  // Update slide image by index.
  void updateSlideImage();

  QLabel* container_label_ = nullptr;
  QPropertyAnimation* pos_animation_ = nullptr;
  QGraphicsOpacityEffect* opacity_effect_ = nullptr;
  QPropertyAnimation* opacity_animation_ = nullptr;
  QParallelAnimationGroup* animation_group_ = nullptr;
  QVariantAnimation* null_animation_ = nullptr;

  int slide_index_;
  QStringList slide_files_;

 private slots:
  // Update slide image while switching to next animation loop
  void onAnimationCurrentLoopChanged();
};

}  // namespace installer

#endif  // INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H
