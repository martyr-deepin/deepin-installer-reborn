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

#ifndef INSTALLER_UI_MODELS_FS_MODEL_H
#define INSTALLER_UI_MODELS_FS_MODEL_H

#include <QAbstractListModel>

#include "partman/fs.h"

namespace installer {

class FsModel : public QAbstractListModel {
  Q_OBJECT

 public:
  FsModel(const FsTypeList& fs_list, QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Get fs type at |index|.
  FsType getFs(int index) const;

  // Index fs type. Returns -1 if not found.
  int index(FsType fs_type) const;

  // If |show_efi| is false, remove efi from fs list.
  void setShowEFI(bool show_efi);

  // If |show_unknown| is true, append it to fs list.
  void setShowUnknown(bool show_unknown);

 private:
  FsTypeList fs_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_FS_MODEL_H
