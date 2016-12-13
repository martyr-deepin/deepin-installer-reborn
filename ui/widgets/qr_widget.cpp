// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/qr_widget.h"

#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <qrencode.h>

namespace installer {

// Refers: http://stackoverflow.com/questions/21400254
QRWidget::QRWidget(QWidget* parent) : QWidget(parent), content_(), margin_(0) {
  this->setObjectName("qr_widget");
}

void QRWidget::setText(const QString& content) {
  content_ = content;
  this->update();
}

void QRWidget::setMargin(int margin) {
  if (margin >= 0) {
    margin_ = margin;
  } else {
    margin_ = 0;
    qWarning() << "setMargin() invalid margin:" << margin;
  }
}

void QRWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  // TODO(xushaohua): Check content_ is empty or not.
  QRcode *qr = QRcode_encodeString(content_.toStdString().c_str(),
                                   1, QR_ECLEVEL_L, QR_MODE_8, 1);
  if (qr != NULL){
    QColor fg("black");
    QColor bg("white");
    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
    painter.setBrush(fg);
    const int s = qr->width > 0 ? qr->width: 1;
    const double w = width() - 2 * margin_;
    const double h = height() - 2 * margin_;
    const double aspect = w / h;
    const double scale = ((aspect > 1.0) ? h : w) / s;
    for(int y = 0; y < s; y++){
      const int yy = y*s;
      for(int x = 0; x < s; x++){
        const int xx = yy + x;
        const unsigned char b = qr->data[xx];
        if(b & 0x01){
          const double rx1 = x * scale + margin_;
          const double ry1 = y * scale + margin_;
          QRectF r(rx1, ry1, scale, scale);
          painter.drawRects(&r, 1);
        }
      }
    }
    QRcode_free(qr);
  }
  else{
    QColor error("red");
    painter.setBrush(error);
    painter.drawRect(0, 0, width(), height());
    qWarning() << "QR FAIL: "<< strerror(errno);
  }
}

}  // namespace installer