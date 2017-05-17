// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/keyboard_layout_variant_model.h"

#include <algorithm>
#include <QCollator>
#include <QDebug>

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

  if (index.isValid()) {
    return variant_list_.at(index.row()).description;
  } else {
    return QVariant();
  }
}

int KeyboardLayoutVariantModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return variant_list_.length();
}

void KeyboardLayoutVariantModel::setVariantList(
    const XKbLayoutVariantList& variant_list, const QString& locale) {
  Q_UNUSED(locale);
  this->beginResetModel();

  variant_list_ = variant_list;

  if (variant_list_.isEmpty()) {
    qCritical() << "VariantList is empty! We shall never reach here!";
    this->endResetModel();
    return;
  }

  // Sort variant list by description.
  const QLocale curr_locale(locale);
  QCollator collator(curr_locale);
  collator.setCaseSensitivity(Qt::CaseInsensitive);

  // Sorting variant list by description, skipping the first item.
  std::sort(variant_list_.begin() + 1, variant_list_.end(),
            [&](const XkbLayoutVariant& a, const XkbLayoutVariant& b) -> bool {
              return collator.compare(a.description, b.description) < 0;
            });

  // Call this method resulting in emitting dataChanged() signal.
  this->endResetModel();
}

QString KeyboardLayoutVariantModel::getVariantDescription(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return variant_list_.at(index.row()).description;
  } else {
    return QString();
  }
}

QString KeyboardLayoutVariantModel::getVariantName(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return variant_list_.at(index.row()).name;
  } else {
    return QString();
  }
}

}  // namespace installer