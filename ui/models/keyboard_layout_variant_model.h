// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
