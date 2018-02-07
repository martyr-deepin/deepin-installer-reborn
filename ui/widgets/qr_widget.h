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

#ifndef INSTALLER_UI_WIDGETS_QR_WIDGET_H
#define INSTALLER_UI_WIDGETS_QR_WIDGET_H

#include <QWidget>
class QPaintEvent;

namespace installer {

// A widget to render string content into QR image
class QRWidget : public QWidget {
  Q_OBJECT

 public:
  explicit QRWidget(QWidget* parent = nullptr);

  // Get current qr content.
  QString text() const { return content_; }

 public slots:
  // Set margin of qr image. |margin| shall be a non-negative integer.
  // Call update() to repaint qr image.
  void setMargin(int margin);

  // Update qr |content|.
  void setText(const QString& content);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QString content_;
  int margin_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_QR_WIDGET_H
