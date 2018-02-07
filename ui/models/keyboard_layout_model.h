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

#ifndef INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_MODEL_H
#define INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_MODEL_H

#include <QAbstractListModel>

#include "sysinfo/keyboard.h"

namespace installer {

// Model used in SystemInfoKeyboardFrame to hold keyboard list.
class KeyboardLayoutModel : public QAbstractListModel {
  Q_OBJECT

 public:
  // Init keyboard layout list here.
  explicit KeyboardLayoutModel(QObject* parent = nullptr);

  // Get model data at |index|.
  QVariant data(const QModelIndex& index, int role) const override;

  // Get number of rows with |parent|.
  int rowCount(const QModelIndex& parent) const override;

  // Get variant list at |index|.
  XKbLayoutVariantList getVariantList(const QModelIndex& index) const;

  // Get layout description at |index|.
  QString getLayoutDescription(const QModelIndex& index) const;

  // Get layout name at |index|.
  QString getLayoutName(const QModelIndex& index) const;

  // Get index of layout with |name|.
  // Result might be invalid.
  QModelIndex getLayoutByName(const QString& name) const;

  // Read xkb layout list with |locale|.
  void initLayout(const QString& locale);

 private:
  XkbConfig xkb_config_;

  // Keyboard layout list sorted by description.
  XkbLayoutList layout_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_KEYBOARD_LAYOUT_MODEL_H
