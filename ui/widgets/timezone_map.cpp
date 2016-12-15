// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/timezone_map.h"

#include <QDebug>
#include <QFontMetrics>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListView>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QStringListModel>
#include <QVBoxLayout>

#include "base/file_util.h"
#include "ui/delegates/timezone_map_util.h"
#include "ui/delegates/timezone_popup_delegate.h"
#include "ui/widgets/tooltip_container.h"
#include "ui/widgets/tooltip_pin.h"

namespace installer {

namespace {

const int kDotVerticalMargin = 2;
const int kZonePinHeight = 30;
const int kZonePinMinimumWidth = 60;

// Height of popup item is also defined in styles/timezone_map.css
const int kPopupItemHeight = 24;
const int kPopupListViewVerticalMargins = 4;
const int kPopupListViewHorizontalMargins = 20;
const int kPopupListViewMinimumWidth = 60;

const double kDistanceThreshold = 100.0;
const char kDotFile[] = ":/images/indicator_active.png";
const char kTimezoneMapFile[] = ":/images/timezone_map_big.png";

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
  connect(popup_window_, &TooltipContainer::onHide,
          dot_, &QLabel::hide);

  // Hide popup_window_ and mark new timezone on map when it is selected
  // in popup_list_view_.
  connect(popup_list_view_, &QListView::pressed,
          this, &TimezoneMap::onPopupZonesActivated);
}

void TimezoneMap::initUI() {
  QLabel* background_label = new QLabel(this);
  background_label->setObjectName("background_label");
  QPixmap timezone_pixmap(kTimezoneMapFile);
  Q_ASSERT(!timezone_pixmap.isNull());
  background_label->setPixmap(timezone_pixmap);

  dot_ = new QLabel(this);
  QPixmap dot_pixmap(kDotFile);
  Q_ASSERT(!dot_pixmap.isNull());
  dot_->setPixmap(dot_pixmap);
  dot_->setFixedSize(dot_pixmap.size());
  dot_->hide();

  zone_pin_ = new TooltipPin(this);
  zone_pin_->setFixedHeight(kZonePinHeight);
  zone_pin_->setMinimumWidth(kZonePinMinimumWidth);
  zone_pin_->hide();

  popup_list_view_ = new QListView();
  popup_list_view_->setObjectName("popup_list_view");
  popup_model_ = new QStringListModel(popup_list_view_);
  popup_list_view_->setModel(popup_model_);
  popup_list_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  popup_list_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  popup_list_view_->setUniformItemSizes(true);
  popup_list_view_->setSelectionMode(QListView::SingleSelection);
  popup_list_view_->setEditTriggers(QListView::NoEditTriggers);
  popup_list_view_->adjustSize();
  TimezonePopupDelegate* popup_delegate =
      new TimezonePopupDelegate(popup_list_view_);
  popup_list_view_->setItemDelegate(popup_delegate);
  popup_list_view_->setMouseTracking(true);
  popup_list_view_->setStyleSheet(ReadFile(":/styles/timezone_map.css"));

  popup_window_ = new TooltipContainer();
  popup_window_->setAttribute(Qt::WA_TranslucentBackground, true);
  popup_window_->setWidget(popup_list_view_);

  popup_window_->hide();

  this->setContentsMargins(0, 0, 0, 0);
  this->setFixedSize(timezone_pixmap.size());
}

void TimezoneMap::popupZoneWindow(const QPoint& pos) {
  // Hide all marks first.
  dot_->hide();
  zone_pin_->hide();
  popup_window_->hide();

  // Popup zone list window.
  QStringList zone_names;
  int item_width = kPopupListViewMinimumWidth;
  const QFontMetrics metrics(popup_list_view_->font());
  for (const ZoneInfo& zone : nearest_zones_) {
    const QString zone_name = GetTimezoneName(zone.timezone);
    zone_names.append(zone_name);
    const int curr_width = metrics.width(zone_name);
    item_width = (curr_width > item_width) ? curr_width : item_width;
  }
  popup_model_->setStringList(zone_names);

  // Add margin to list view.
  const int width = item_width + kPopupListViewHorizontalMargins * 2;
  const int height = kPopupItemHeight * zone_names.length() +
                     kPopupListViewVerticalMargins * 2;
  popup_list_view_->resize(width, height);
  dot_->move(pos.x() - dot_->width() / 2, pos.y() - dot_->height() / 2);
  dot_->show();

  const int dy = pos.y() - dot_->height() - kDotVerticalMargin;
  const QPoint global_pos = this->mapToGlobal(QPoint(pos.x(), dy));
  popup_window_->popup(global_pos);
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
    // TODO(xushaohua): Convert timezone to other names.
    zone_pin_->setText(GetTimezoneName(current_zone_.timezone));

    // Adjust size of pin to fit its content.
    zone_pin_->adjustSize();

    // Show zone pin at current marked zone.
    const QPoint point = ZoneInfoToPosition(current_zone_,
                                            map_width,
                                            map_height);
    const int dy = point.y() - dot_->height() / 2 - kDotVerticalMargin;
    zone_pin_->popup(QPoint(point.x(), dy));

    dot_->move(point.x() - dot_->width() / 2,
               point.y() - dot_->height() / 2);
    dot_->show();
  }
}

void TimezoneMap::onPopupZonesActivated(const QModelIndex& index) {
// Hide popup_window_.
  popup_window_->hide();
  dot_->hide();

  // Update current selected timezone and mark it on map.
  Q_ASSERT(index.isValid());
  if (index.isValid()) {
    const int row = index.row();
    Q_ASSERT(row < nearest_zones_.length());
    if (row < nearest_zones_.length()) {
      current_zone_ = nearest_zones_.at(row);
      this->remark();
      emit this->timezoneUpdated(current_zone_.timezone);
    }
  }
}

}  // namespace installer
