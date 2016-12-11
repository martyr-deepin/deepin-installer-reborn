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

const double kDistanceThreshold = 100.0;

}  // namespace

TimezoneMap::TimezoneMap(QWidget* parent)
    : QFrame(parent),
      timezone_(),
      background_pixmap_(":/images/timezone_map_big.png"),
      total_zones_(GetZoneInfoList()),
      nearest_zones_(),
      bubble_mode_(BubbleMode::Nil) {
  this->setObjectName("timezone_map");
  this->setFixedSize(background_pixmap_.size());
}

const QString TimezoneMap::getTimezone() const {
  return timezone_;
}

void TimezoneMap::setTimezone(const QString& timezone) {
  timezone_ = timezone;
  bubble_mode_ = BubbleMode::Set;
  this->update();
  emit this->timezoneUpdated(timezone_);
}

void TimezoneMap::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    nearest_zones_ = GetNearestZones(total_zones_, kDistanceThreshold,
                                     event->x(), event->y(),
                                     this->width(), this->height());
    qDebug() << nearest_zones_;
    bubble_mode_ = (nearest_zones_.length() == 1) ?
                   BubbleMode::SelectSingle :
                   BubbleMode::SelectMultiple;
    this->update();
  } else {
    QWidget::mousePressEvent(event);
  }
}

void TimezoneMap::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  const int width = this->width();
  const int height = this->height();
  painter.drawPixmap(0, 0, width, height, background_pixmap_);

  const QPixmap pin_pixmap(":/images/timezone_map_pin.png");
  const int pin_width = pin_pixmap.width();
  const int pin_height = pin_pixmap.height();
  for (const ZoneInfo& info : nearest_zones_) {
    qDebug() << "draw zone info" << info << pin_pixmap;
    const int point_y = int(ConvertLatitudeToY(info.latitude) * height);
    const int point_x = int(ConvertLongitudeToX(info.longitude) * width);
    const int x = point_x - pin_width / 2;
    const int y = point_y - pin_height;
    painter.drawPixmap(x, y, pin_width, pin_height, pin_pixmap);
  }

}

}  // namespace installer