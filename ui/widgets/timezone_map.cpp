// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/timezone_map.h"

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>

#include "ui/delegates/timezone_map_util.h"
#include "ui/widgets/tooltip_pin.h"

namespace installer {

namespace {

const double kDistanceThreshold = 100.0;
const char kTimezoneMapFile[] = ":/images/timezone_map.png";

}  // namespace

TimezoneMap::TimezoneMap(QWidget* parent)
    : QFrame(parent),
      timezone_(),
      total_zones_(GetZoneInfoList()),
      nearest_zones_(),
      bubble_mode_(BubbleMode::Nil) {
  this->setObjectName("timezone_map");

  this->initUI();
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

void TimezoneMap::initUI() {
  QLabel* background_label = new QLabel(this);
  background_label->setObjectName("background_label");
  QPixmap timezone_pixmap(kTimezoneMapFile);
  Q_ASSERT(!timezone_pixmap.isNull());
  background_label->setPixmap(timezone_pixmap);

  zone_pin_ = new TooltipPin(this);
  zone_pin_->hide();

  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedSize(timezone_pixmap.size());
}

}  // namespace installer