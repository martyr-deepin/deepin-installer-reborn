// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/utils/widget_util.h"

#include <QComboBox>
#include <QDebug>
#include <QMenu>

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
    QWidget* item = qobject_cast<QWidget*>(child);
    if (item) {
      WidgetTreeWalk(item, indent + 2);
    }
  }
}

}  // namespace

bool SetChildTransparent(QWidget* root, const QString& child_name) {
  for (QObject* child : root->children()) {
    qDebug() << child;
    if (child_name == child->metaObject()->className() || true) {
      QWidget* container = qobject_cast<QWidget*>(child);
      if (container) {
        container->setAttribute(Qt::WA_TranslucentBackground, true);
        return true;
      }
    }
  }
  return false;
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

}  // namespace installer