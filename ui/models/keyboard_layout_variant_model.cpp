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