// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_MISC_OEM_SECTION_LABEL_H
#define DEEPIN_INSTALLER_REBORN_MISC_OEM_SECTION_LABEL_H

#include <QLabel>

class SectionLabel : public QLabel {
  Q_OBJECT

 public:
  SectionLabel(const QString& text, QWidget* parent = nullptr);
};


#endif  // DEEPIN_INSTALLER_REBORN_MISC_OEM_SECTION_LABEL_H
