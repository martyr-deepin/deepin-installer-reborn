// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_TIMEZONE_MAP_H
#define INSTALLER_UI_WIDGETS_TIMEZONE_MAP_H

#include <QFrame>
class QLabel;

#include "sysinfo/timezone.h"

namespace installer {

class TooltipPin;

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
  void mousePressEvent(QMouseEvent* event) override;

 private:
  void initUI();
  // Redraw bubble based on current mode.
  void remark();

  QString timezone_;
  const ZoneInfoList total_zones_;
  ZoneInfoList nearest_zones_;

  // A round dot to indicate position on the map.
  QLabel* dot_ = nullptr;

  TooltipPin* zone_pin_ = nullptr;

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
