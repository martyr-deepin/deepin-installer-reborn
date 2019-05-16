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

#include "ui/widgets/timezone_map.h"

#include <QDebug>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListView>
#include <QMouseEvent>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "../utils/widget_util.h"
#include "service/settings_manager.h"
#include "ui/delegates/timezone_map_util.h"
#include "ui/widgets/popup_menu.h"
#include "ui/widgets/tooltip_pin.h"

namespace installer {

namespace {

static const int kZonePinHeight = 30;
static const int kZonePinMinimumWidth = 60;

static const double kDistanceThreshold = 100.0;
static const QString kDotFile = ":/images/indicator_active.svg";
static const QString kTimezoneMapFile = ":/images/map.svg";

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
      current_zone_(),
      total_zones_(GetZoneInfoList()),
      nearest_zones_() {
  this->setObjectName("timezone_map");

  this->initUI();
  this->initConnections();
}

TimezoneMap::~TimezoneMap() {
  if (popup_window_) {
    delete popup_window_;
    popup_window_ = nullptr;
  }
}

const QString TimezoneMap::getTimezone() const {
  return current_zone_.timezone;
}

void TimezoneMap::setTimezone(const QString& timezone) {
  nearest_zones_.clear();
  const int index = GetZoneInfoByZone(total_zones_, timezone);
  if (index > -1) {
    current_zone_ = total_zones_.at(index);
    nearest_zones_.append(current_zone_);
    this->remark();
  } else {
    qWarning() << "Timezone not found:" << timezone;
  }
}

void TimezoneMap::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    // Get nearest zones around mouse.
    nearest_zones_ = GetNearestZones(total_zones_, kDistanceThreshold,
                                     event->x(), event->y(),
                                     this->width(), this->height());
    qDebug() << nearest_zones_;
    if (nearest_zones_.length() == 1) {
      current_zone_ = nearest_zones_.first();
      this->remark();
      emit this->timezoneUpdated(current_zone_.timezone);
    } else {
      this->popupZoneWindow(event->pos());
    }
  } else {
    QWidget::mousePressEvent(event);
  }
}

void TimezoneMap::resizeEvent(QResizeEvent* event) {
  if (popup_window_->isVisible()) {
    dot_->hide();
    popup_window_->hide();
  }
  QWidget::resizeEvent(event);
}

void TimezoneMap::initConnections() {
  // Hide dot when popup-zones window is hidden.
  connect(popup_window_, &PopupMenu::onHide,
          dot_, &QLabel::hide);

  // Hide popup_window_ and mark new timezone on map.
  connect(popup_window_, &PopupMenu::menuActivated,
          this, &TimezoneMap::onPopupWindowActivated);
}

void TimezoneMap::initUI() {
  QLabel* background_label = new QLabel(this);
  background_label->setObjectName("background_label");
  QPixmap timezone_pixmap = std::move(installer::renderPixmap(kTimezoneMapFile));
  Q_ASSERT(!timezone_pixmap.isNull());
  background_label->setPixmap(timezone_pixmap);

  Q_ASSERT(this->parentWidget());
  // Set parent widget of dot_ to TimezoneFrame.
  dot_ = new QLabel(this->parentWidget());
  const QPixmap dot_pixmap = std::move(installer::renderPixmap(kDotFile));
  Q_ASSERT(!dot_pixmap.isNull());
  dot_->setPixmap(dot_pixmap);
  dot_->setFixedSize(dot_pixmap.size() / devicePixelRatioF());
  dot_->hide();

  // Set parent widget of zone_pin_ to TimezoneFrame.
  zone_pin_ = new TooltipPin(this->parentWidget());
  zone_pin_->setFixedHeight(kZonePinHeight);
  zone_pin_->setMinimumWidth(kZonePinMinimumWidth);
  // Allow mouse event to pass through.
  zone_pin_->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  zone_pin_->hide();

  popup_window_ = new PopupMenu(this->parentWidget());
  popup_window_->hide();

  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedSize(timezone_pixmap.size() / devicePixelRatioF());
}

void TimezoneMap::popupZoneWindow(const QPoint& pos) {
  // Hide all marks first.
  dot_->hide();
  zone_pin_->hide();
  popup_window_->hide();

  // Popup zone list window.
  QStringList zone_names;
  const QString locale = ReadLocale();
  for (const ZoneInfo& zone : nearest_zones_) {
    zone_names.append(GetLocalTimezoneName(zone.timezone, locale));
  }

  // Show popup window above dot
  popup_window_->setStringList(zone_names);

  const int half_width = dot_->width() / 2;
  const int half_height = dot_->height() / 2;
  // Position relative to parent.
  const QPoint parent_pos(this->mapToParent(pos));

  // Add 8px margin.
  const QPoint popup_pos(parent_pos.x(), parent_pos.y() - half_height - 8);
  popup_window_->popup(popup_pos);

  const QPoint dot_pos(parent_pos.x() - half_width,
                       parent_pos.y() - half_height);
  dot_->move(dot_pos);
  dot_->show();
}

void TimezoneMap::remark() {
  // Hide all marks first.
  dot_->hide();
  zone_pin_->hide();
  popup_window_->hide();

  const int map_width = this->width();
  const int map_height = this->height();

  Q_ASSERT(!nearest_zones_.isEmpty());
  if (!nearest_zones_.isEmpty()) {
    const QString locale = ReadLocale();
    zone_pin_->setText(GetLocalTimezoneName(current_zone_.timezone, locale));

    // Adjust size of pin to fit its content.
    zone_pin_->adjustSize();

    // Show zone pin at current marked zone.
    const QPoint zone_pos = ZoneInfoToPosition(current_zone_, map_width,
                                               map_height);

    const int half_width = dot_->width() / 2;
    const int half_height = dot_->height() / 2;
    const QPoint parent_pos(this->mapToParent(zone_pos));

    // Add 2px margin.
    const QPoint zone_pin_pos(parent_pos.x(), parent_pos.y() - half_height - 2);
    zone_pin_->popup(zone_pin_pos);

    const QPoint dot_pos(parent_pos.x() - half_width,
                         parent_pos.y() - half_height);
    dot_->move(dot_pos);
    dot_->show();
  }
}

void TimezoneMap::onPopupWindowActivated(int index) {
  // Hide popup window and dot first.
  popup_window_->hide();
  dot_->hide();

  // Update current selected timezone and mark it on map.
  Q_ASSERT(index < nearest_zones_.length());
  if (index < nearest_zones_.length()) {
    current_zone_ = nearest_zones_.at(index);
    this->remark();
    emit this->timezoneUpdated(current_zone_.timezone);
  }
}

}  // namespace installer
