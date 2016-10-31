// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/widgets/simple_partition_button.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPen>
#include <QPixmap>

#include "base/file_util.h"
#include "ui/delegates/partition_util.h"
#include "ui/widgets/partition_usage_bar.h"

namespace installer {

namespace {

const int kButtonWidth = 220;
const int kButtonHeight = 220;
const int kBorderRadius = 15;
const int kBorderDiameter = kBorderRadius * 2;
const int kVerticalSpacing = 15;
const int kOsIconWidth = 120;
const int kOsIconHeight = 120;
const char kPathColor[] = "#e8e8e8";
const char kUsageColor[] = "#c8c8c8";

QString GetImageByOsType(OsType os_type) {
  switch (os_type) {
    case OsType::Empty: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Linux: {
      return QStringLiteral(":/images/driver_linux_128.png");
    }
    case OsType::Mac: {
      return QStringLiteral(":/images/driver_mac_128.png");
    }
    case OsType::Unknown: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Windows: {
      return QStringLiteral(":/images/driver_windows_128.png");
    }
    default: {
      return QString();
    }
  }
}

}  // namespace

SimplePartitionButton::SimplePartitionButton(const Partition& partition,
                                             QWidget* parent)
    : QPushButton(parent),
      partition_(partition) {
  this->setObjectName(QStringLiteral("simple_partition_button"));
  this->setFixedSize(kButtonWidth, kButtonHeight);
}

void SimplePartitionButton::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);

  const QRect rect(this->rect());

  // First draw semi-transparent background with round corner.
  const QColor background_color(QColor::fromRgb(255, 255, 255, 10));
  QPainterPath background_path;
  background_path.moveTo(kButtonWidth, kBorderRadius);
  background_path.arcTo(kButtonWidth - kBorderDiameter, 0,
                        kBorderDiameter, kBorderDiameter,
                        0.0, 90.0);
  background_path.lineTo(kBorderRadius, 0);
  background_path.arcTo(0, 0, kBorderDiameter, kBorderDiameter, 90.0, 90.0);
  background_path.lineTo(0, kButtonHeight - kBorderRadius);
  background_path.arcTo(0, kButtonHeight - kBorderDiameter,
                        kBorderDiameter, kBorderDiameter,
                        180.0, 90.0);
  background_path.lineTo(kButtonWidth - kBorderRadius, kButtonHeight);
  background_path.arcTo(kButtonWidth - kBorderDiameter,
                        kButtonHeight - kBorderDiameter,
                        kBorderDiameter, kBorderDiameter,
                        270.0, 90.0);
  background_path.closeSubpath();
  painter.fillPath(background_path, QBrush(background_color));

  // Then draw os image
  const QRect os_rect((rect.width() - kOsIconWidth) / 2, 0,
                      kOsIconWidth, kOsIconHeight);
  QPixmap os_pixmap(GetImageByOsType(partition_.os));
  if (os_pixmap.width() != kOsIconWidth) {
    // Scale image.
    // TODO(xushaohua): Cache os icon
    // TODO(xushaohua): read chcked property
    os_pixmap = os_pixmap.scaled(kOsIconWidth, kOsIconHeight);
  }
  painter.drawPixmap(os_rect, os_pixmap);

  // Draw partition label/path
  const QString label_text = GetPartitionLabelAndPath(partition_);
  const QColor label_color(QColor::fromRgb(255, 255, 255, 255));
  QFont label_font;
  label_font.setPixelSize(14);
  const QFontMetrics label_font_metrics(label_font);
  const int label_length = label_font_metrics.width(label_text);
  painter.setPen(QPen(label_color));
  painter.setFont(label_font);
  // Set text alignment to center
  painter.drawText((kButtonWidth - label_length) / 2,
                   kOsIconHeight + kVerticalSpacing,
                   label_text);

  // Draw partition usage text
  qDebug() << partition_.length << partition_.freespace
           << partition_.getByteLength() << partition_.getSectorLength();
  const QString usage_text = GetPartitionUsage(partition_);
  const QColor usage_color(QColor::fromRgb(255, 255, 255, 120));
  QFont usage_font;
  usage_font.setPixelSize(10);
  const QFontMetrics usage_font_metrics(usage_font);
  const int usage_length = usage_font_metrics.width(usage_text);
  painter.setFont(usage_font);
  painter.setPen(QPen(usage_color));
  painter.drawText((kButtonWidth - usage_length) / 2,
                   kOsIconHeight + kVerticalSpacing * 2,
                   usage_text);

  // Draw partition usage progress bar
}

}  // namespace installer
