// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/system_info_tip.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

namespace installer {

namespace {

const int kWindowTopMargin = 5;

const int kBorderRadius = 5;
const int kBorderDiameter = kBorderRadius * 2;

// _____
// \ | /
//  \|/
const int kTriangleHeight = 6;
const int kTriangleWidth = 10;
const int kTriangleHalfWidth = kTriangleWidth / 2;
const int kTriangleLeftMargin = 10;

}  // namespace

SystemInfoTip::SystemInfoTip(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("system_info_tip"));

  label_ = new QLabel();
  label_->setStyleSheet("background: transparent;"
                        "color: #f9704f;"
                        "font-size: 14px;");
  label_->setWordWrap(true);
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(label_);

  this->setLayout(layout);
  this->setContentsMargins(10, 15, 10, 10);
}

void SystemInfoTip::setText(const QString& text) {
  label_->setText(text);
  this->adjustSize();
}

void SystemInfoTip::showBottom(QWidget* widget) {
  this->move(widget->x(), widget->y() + widget->height() + kWindowTopMargin);
  this->raise();
  this->show();
}

void SystemInfoTip::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing,
                         true);
  const int kWidth = this->width();
  const int kHeight = this->height();

  // First draw background with round corner.
  QPainterPath background_path;
  background_path.moveTo(kWidth, kBorderRadius + kTriangleHeight);
  background_path.arcTo(kWidth - kBorderDiameter, kTriangleHeight,
                        kBorderDiameter, kBorderDiameter,
                        0.0, 90.0);
  // Draw isosceles right-angled triangle 10px to top left side
  background_path.lineTo(kTriangleLeftMargin + kTriangleWidth, kTriangleHeight);
  background_path.lineTo(kTriangleLeftMargin + kTriangleHeight, 0);
  background_path.lineTo(kTriangleLeftMargin, kTriangleHeight);
  background_path.lineTo(kBorderRadius, kTriangleHeight);
  background_path.arcTo(0, kTriangleHeight,
                        kBorderDiameter, kBorderDiameter,
                        90.0, 90.0);
  background_path.lineTo(0, kHeight - kBorderRadius);
  background_path.arcTo(0, kHeight - kBorderDiameter,
                        kBorderDiameter, kBorderDiameter,
                        180.0, 90.0);
  background_path.lineTo(kWidth - kBorderRadius, kHeight);
  background_path.arcTo(kWidth - kBorderDiameter, kHeight - kBorderDiameter,
                        kBorderDiameter, kBorderDiameter,
                        270.0, 90.0);
  background_path.closeSubpath();
  painter.fillPath(background_path, QBrush(Qt::white));
}

}  // namespace installer