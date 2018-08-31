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

#include "ui/utils/widget_util.h"

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QLayout>
#include <QLayoutItem>
#include <QMenu>
#include <QImageReader>
#include <QApplication>
#include <QPixmap>

namespace installer {

namespace {

void WidgetTreeWalk(QWidget* root, int indent) {
  char prefix[indent+1];
  for (int i = 0; i < indent; i++) {
    prefix[i] = '*';
  }
  prefix[indent] = '\0';
  qDebug() << prefix << root;
  for (QObject* child : root->children()) {
    QWidget* item = dynamic_cast<QWidget*>(child);
    if (item) {
      WidgetTreeWalk(item, indent + 2);
    }
  }
}

}  // namespace

void AppendStyleSheet(QWidget* widget, const QString& style) {
  widget->setStyleSheet(widget->styleSheet() + style);
  widget->ensurePolished();
}

void ClearLayout(QLayout* layout) {
  for (QLayoutItem* item = layout->takeAt(0);
       item != nullptr;
       item = layout->takeAt(0)) {
    delete item->widget();
    delete item;
    item = nullptr;
  }
}

bool SetChildTransparent(QWidget* root, const QString& child_name) {
  for (QObject* child : root->children()) {
    if (child_name == child->metaObject()->className()) {
      QWidget* container = dynamic_cast<QWidget*>(child);
      if (container) {
        container->setAttribute(Qt::WA_TranslucentBackground, true);
        return true;
      }
    }
  }
  return false;
}

void ShowFullscreen(QWidget* widget) {
  // NOTE(xushaohua): If geometry of primary screen changes too fast, this
  // function may return false screen geometry.
  const QRect rect = qApp->desktop()->screenGeometry();
  ShowFullscreen(widget, rect);
  widget->showFullScreen();
}

void ShowFullscreen(QWidget* widget, const QRect& geometry) {
  qDebug() << "ShowFullscreen()" << geometry;
  widget->move(geometry.topLeft());
  widget->setFixedSize(geometry.size());
}

void SetQComboBoxTransparent(QComboBox* box) {
  if (!SetChildTransparent(box, "QComboBoxPrivateContainer")) {
    qWarning() << "SetQComboBoxTransparent() failed to find private container!";
  }
}

void SetQMenuTransparent(QMenu* menu) {
  if (!SetChildTransparent(menu, "QMenuPrivate")) {
    qWarning() << "SetQMenuTransparent() failed to find private container!";
  }
}

void WidgetTreeWalk(QWidget* root) {
  WidgetTreeWalk(root, 0);
}

const QPixmap renderPixmap(const QString &path) {
    QImageReader reader;
    QPixmap pixmap;
    reader.setFileName(path);
    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(reader.size() * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    }
    else {
        pixmap.load(path);
    }

    return pixmap;
}

}  // namespace installer
