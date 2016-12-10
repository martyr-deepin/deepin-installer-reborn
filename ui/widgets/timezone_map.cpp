// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/timezone_map.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

#include "ui/delegates/timezone_map_util.h"

namespace installer {

namespace {

const int kDistanceThreshold = 100;

}  // namespace

TimezoneMap::TimezoneMap(QWidget* parent)
    : QFrame(parent),
      timezone_(),
      background_pixmap_(":/images/timezone_map_big.png"),
      total_zones_(GetZoneInfoList()) {
  this->setObjectName("timezone_map");
  this->setFixedSize(background_pixmap_.size());
}

const QString TimezoneMap::getTimezone() const {
  return timezone_;
}

void TimezoneMap::setTimezone(const QString& timezone) {
  timezone_ = timezone;
  this->update();
  emit this->timezoneUpdated(timezone_);
}

void TimezoneMap::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    const ZoneInfoList zones = GetNearestZones(total_zones_, kDistanceThreshold,
                                               event->x(), event->y(),
                                               this->width(), this->height());
    qDebug() << zones;
  } else {
    QWidget::mousePressEvent(event);
  }
}

void TimezoneMap::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.drawPixmap(0, 0, this->width(), this->height(), background_pixmap_);

}

}  // namespace installer