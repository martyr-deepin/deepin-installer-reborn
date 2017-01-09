// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
