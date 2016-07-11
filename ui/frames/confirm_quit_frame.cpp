// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/confirm_quit_frame.h"

namespace ui {

ConfirmQuitFrame::ConfirmQuitFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName(QStringLiteral("confirm_quit_frame"));

  this->initUI();
}

void ConfirmQuitFrame::initUI() {
  this->setAttribute(Qt::WA_TranslucentBackground, true);
}

}  // namespace ui