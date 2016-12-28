// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/keyboard_layout_model.h"

#include <algorithm>
#include <QCollator>

namespace installer {

KeyboardLayoutModel::KeyboardLayoutModel(QObject* parent)
    : QAbstractListModel(parent),
      xkb_config_(),
      layout_list_() {
  this->setObjectName("keyboard_layout_model");
}

QVariant KeyboardLayoutModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    return layout_list_.at(index.row()).description;
  } else {
    return QVariant();
  }
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

void KeyboardLayoutModel::initLayout() {
  this->beginResetModel();

  // Load xkb layout based on current locale.
  // Locale environment is setup in SelectLanguageFrame.
  xkb_config_ = GetXkbConfig();
  layout_list_ = xkb_config_.layout_list;

  // Append layout to its variant list.
  for (XkbLayout& layout : layout_list_) {
    XkbLayoutVariant variant;
    variant.name = layout.name;
    variant.description = layout.description;
    variant.short_description = layout.short_description;
    variant.language_list = layout.language_list;
    layout.variant_list.prepend(variant);
  }

  // Sort layout list by description.
  // Perform localized comparison.
  const QCollator collator(QLocale(qgetenv("LC_ALL")));
  std::sort(layout_list_.begin(), layout_list_.end(),
            [&](const XkbLayout& a, const XkbLayout& b) -> bool {
              return collator.compare(a.description, b.description) < 0;
            });

  this->endResetModel();
}

}  // namespace installer