// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_MISC_OEM_SECTION_LABEL_H
#define INSTALLER_MISC_OEM_SECTION_LABEL_H

#include <QLabel>

namespace installer {

class SectionLabel : public QLabel {
  Q_OBJECT

 public:
  SectionLabel(const QString& text, QWidget* parent = nullptr);
};

}  // namespace installer

#endif  // INSTALLER_MISC_OEM_SECTION_LABEL_H
