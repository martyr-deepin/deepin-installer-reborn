// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_MODELS_LANGUAGE_LIST_MODEL_H
#define DEEPIN_INSTALLER_REBORN_UI_MODELS_LANGUAGE_LIST_MODEL_H

#include <QAbstractListModel>
#include <service/system_language.h>

namespace ui {

class LanguageListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit LanguageListModel(QObject* parent = nullptr);

  virtual int rowCount(const QModelIndex& parent) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

  // Get locale name at |index|.
  QString locale(const QModelIndex& index) const;

 private:
  service::LanguageList lang_list_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_MODELS_LANGUAGE_LIST_MODEL_H
