// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/keyboard_layout_model.h"

#include <algorithm>

namespace installer {

KeyboardLayoutModel::KeyboardLayoutModel(QObject* parent)
    : QAbstractListModel(parent),
      xkb_config_(GetXkbConfig()),
      layout_list_() {
  this->setObjectName("keyboard_layout_model");

  layout_list_ = xkb_config_.layout_list;
  // Sort layout list by description.
  std::sort(layout_list_.begin(), layout_list_.end());
}

QVariant KeyboardLayoutModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const int row = index.row();
  if (row >= layout_list_.length()) {
    return QVariant();
  }

  return layout_list_.at(row).description;
}

int KeyboardLayoutModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return layout_list_.length();
}

XKbLayoutVariantList KeyboardLayoutModel::getVariantList(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return layout_list_.at(index.row()).variant_list;
  } else {
    return XKbLayoutVariantList();
  }
}

QString KeyboardLayoutModel::getLayoutDescription(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return layout_list_.at(index.row()).description;
  } else {
    return QString();
  }
}

QString KeyboardLayoutModel::getLayoutName(const QModelIndex& index) const {
  if (index.isValid()) {
    return layout_list_.at(index.row()).name;
  } else {
    return QString();
  }
}

QModelIndex KeyboardLayoutModel::getLayoutByName(const QString& name) const {
  for (int row = 0; row < layout_list_.length(); ++row) {
    if (layout_list_.at(row).name == name) {
      return this->index(row);
    }
  }
  return QModelIndex();
}

}  // namespace installer