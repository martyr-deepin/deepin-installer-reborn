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

#ifndef INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_VARIANT_MODEL_H
#define INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_VARIANT_MODEL_H

#include <QAbstractListModel>

#include "sysinfo/keyboard.h"

namespace installer {

// Model used in SystemInfoKeyboardFrame to hold keyboard layout variant list.
class KeyboardLayoutVariantModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit KeyboardLayoutVariantModel(QObject* parent = nullptr);

  // Get model data at |index|.
  QVariant data(const QModelIndex& index, int role) const override;

  // Get number of rows with |parent|.
  int rowCount(const QModelIndex& parent) const override;

  // Set variant list and emit dataChanged() signal.
  void setVariantList(const XKbLayoutVariantList& variant_list,
                      const QString& locale);

  // Get description at |index|.
  QString getVariantDescription(const QModelIndex& index) const;

  // Get layout variant name at |index|.
  QString getVariantName(const QModelIndex& index) const;

 private:
  XKbLayoutVariantList variant_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_VARIANT_MODEL_H
