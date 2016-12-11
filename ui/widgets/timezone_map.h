// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_TIMEZONE_MAP_H
#define INSTALLER_UI_WIDGETS_TIMEZONE_MAP_H

#include <QFrame>
#include <QPixmap>

#include "sysinfo/timezone.h"

namespace installer {

// Draw timezone map and bubble.
class TimezoneMap : public QFrame {
  Q_OBJECT

 public:
  explicit TimezoneMap(QWidget* parent = nullptr);

  // Get current selected timezone, might be empty.
  const QString getTimezone() const;

 signals:
  void timezoneUpdated(const QString& timezone);

 public slots:
  void setTimezone(const QString& timezone);

 protected:
  void mousePressEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent* event);

 private:
  QString timezone_;
  const QPixmap background_pixmap_;

  const ZoneInfoList total_zones_;
  ZoneInfoList nearest_zones_;

  enum class BubbleMode {
    Nil,
    Set,  // When setTimezone() is called.
    SelectSingle,  // When there is only one zone near cursor position.
    SelectMultiple,  // When there are several zones near cursor position.
  };
  BubbleMode bubble_mode_;
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_TIMEZONE_MAP_H
