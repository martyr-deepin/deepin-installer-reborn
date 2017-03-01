// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SPINNER_LABEL_H_
#define INSTALLER_UI_WIDGETS_SPINNER_LABEL_H_

#include <QLabel>
class QTimer;

namespace installer {

// Displays spinner animation.
class SpinnerLabel : public QLabel {
  Q_OBJECT

 public:
  explicit SpinnerLabel(QWidget* parent = nullptr);

  // Check whether spinner animation is active.
  bool isActive() const;

 public slots:
  // start animation;
  void start();

  // Stop animation;
  void stop();

 private:
  QTimer* animation_timer_ = nullptr;
  // Cache spinner pixmaps.
  QVector<QPixmap> pixmaps_;
  int pixmap_index_;

 private slots:
  void onAnimationTimerTimeout();
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SPINNER_LABEL_H_
