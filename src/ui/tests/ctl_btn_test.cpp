/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include <QApplication>
#include <QDebug>

#include "base/file_util.h"
#include "ui/widgets/pointer_button.h"

namespace {

const char kStyleFile[] = ":/styles/advanced_partition_button.css";

const char kCtlBtnState[] = "ctlState";
const char kCtlBtnStateDelete[] = "delete";
const char kCtlBtnStateEdit[] = "edit";
const char kCtlBtnStateNew[] = "new";
const char kCtlBtnStateHide[] = "hide";

}  // namespace

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  installer::PointerButton ctl_btn;
  ctl_btn.setObjectName("control_button");
  ctl_btn.setStyleSheet(installer::ReadFile(":/styles/advanced_partition_button.css"));
  ctl_btn.setFixedSize(18, 18);
  ctl_btn.setProperty(kCtlBtnState, kCtlBtnStateNew);
  ctl_btn.setFlat(true);
  ctl_btn.show();

  return app.exec();
}