// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_WIDGETS_SECTION_LABEL_H
#define INSTALLER_UI_WIDGETS_SECTION_LABEL_H

#include <QLabel>

namespace installer {

// TODO(xushaohua): Remove this class.
class SectionLabel : public QLabel {
  Q_OBJECT

 public:
  SectionLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_UI_WIDGETS_SECTION_LABEL_H
