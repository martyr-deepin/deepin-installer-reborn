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
