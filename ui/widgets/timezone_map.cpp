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

const int kZonePinHeight = 30;

const double kDistanceThreshold = 100.0;
const char kTimezoneMapFile[] = ":/images/timezone_map.png";

// At absolute position of |zone| on a map with size (map_width, map_height).
QPoint ZoneInfoToPosition(const ZoneInfo& zone, int map_width, int map_height) {
  // TODO(xushaohua): Call round().
  const int x = int(ConvertLongitudeToX(zone.longitude) * map_width);
  const int y = int(ConvertLatitudeToY(zone.latitude) * map_height);
  return QPoint(x, y);
}

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
    this->remark();
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
  zone_pin_->setFixedHeight(kZonePinHeight);
  zone_pin_->hide();

  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedSize(timezone_pixmap.size());
}

void TimezoneMap::remark() {
  const int map_width = this->width();
  const int map_height = this->height();
  switch (bubble_mode_) {
    case BubbleMode::SelectSingle: {
      Q_ASSERT(!nearest_zones_.isEmpty());
      if (!nearest_zones_.isEmpty()) {
        const ZoneInfo& zone = nearest_zones_.first();
        // TODO(xushaohua): Convert timezone to other names.
        zone_pin_->setText(GetTimezoneName(zone.timezone));

        // Adjust size of pin to fit its content.
        zone_pin_->adjustSize();

        // Show zone pin at the nearest zone.
        zone_pin_->popup(ZoneInfoToPosition(zone, map_width, map_height));
      }
      break;
    }
    case BubbleMode::SelectMultiple: {
      break;
    }
    case BubbleMode::Set: {
      break;
    }
    default: {
      // Pass through.
      break;
    }
  }
}

}  // namespace installer