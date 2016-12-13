// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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

  QString text() const { return content_; }
  void setText(const QString& content);

 public slots:
  // Set margin of qr image. |margin| shall be a non-negative integer.
  // Call update() to repaint qr image.
  void setMargin(int margin);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  QString content_;
  int margin_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_QR_WIDGET_H
