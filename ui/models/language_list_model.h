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

#ifndef INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H
#define INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H

#include <QAbstractListModel>

#include "service/system_language.h"

namespace installer {

// Reimplemented model used in SelectLanguageFrame
class LanguageListModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit LanguageListModel(QObject* parent = nullptr);

  virtual QVariant data(const QModelIndex& index, int role) const override;
  virtual int rowCount(const QModelIndex& parent) const override;

  // Get language item at |index|.
  LanguageItem languageItemAt(const QModelIndex& index) const;

  // Get index with |locale|.
  QModelIndex localeIndex(const QString& locale) const;

 private:
  LanguageList lang_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_LANGUAGE_LIST_MODEL_H
