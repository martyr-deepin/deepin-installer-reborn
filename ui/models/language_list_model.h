// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H
#define INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H

#include <QAbstractListModel>

#include "service/system_language.h"

namespace installer {

class LanguageListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit LanguageListModel(QObject* parent = nullptr);

  virtual int rowCount(const QModelIndex& parent) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

  // Get locale name at |index|.
  QString locale(const QModelIndex& index) const;

 private:
  LanguageList lang_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H
