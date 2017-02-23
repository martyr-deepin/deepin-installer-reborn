// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  void setLocale(const QString& locale);

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

  QString locale_;
  int slide_index_;
  QStringList slide_files_;

 private slots:
  // Update slide image while switching to next animation loop
  void onAnimationCurrentLoopChanged();
};

}  // namespace installer

#endif  // INSTALLER_INSTALL_PROGRESS_SLIDE_FRAME_H
