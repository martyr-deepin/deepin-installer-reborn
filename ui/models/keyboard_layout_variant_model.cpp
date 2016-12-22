// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/keyboard_layout_variant_model.h"

#include <algorithm>
#include <QCollator>

namespace installer {

KeyboardLayoutVariantModel::KeyboardLayoutVariantModel(QObject* parent)
    : QAbstractListModel(parent),
      variant_list_() {
  this->setObjectName("keyboard_layout_variant_model");
}

QVariant KeyboardLayoutVariantModel::data(const QModelIndex& index,
                                          int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const int row = index.row();
  if (row >= variant_list_.length()) {
    return QVariant();
  }

  return variant_list_.at(row).description;}

int KeyboardLayoutVariantModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return variant_list_.length();
}

void KeyboardLayoutVariantModel::setVariantList(
    const XKbLayoutVariantList& variant_list) {
  this->beginResetModel();

  variant_list_ = variant_list;

  // Sort variant list by description.
  const QCollator collator(QLocale(qgetenv("LC_ALL")));
  std::sort(variant_list_.begin(), variant_list_.end(),
            [&](const XkbLayoutVariant& a, const XkbLayoutVariant& b) -> bool {
              return collator.compare(a.description, b.description) < 0;
            });

  // Call this method resulting in emitting dataChanged() signal.
  this->endResetModel();
}

QString KeyboardLayoutVariantModel::getVariantName(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return variant_list_.at(index.row()).name;
  } else {
    return QString();
  }}

}  // namespace installer