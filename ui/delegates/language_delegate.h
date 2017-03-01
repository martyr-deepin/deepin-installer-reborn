// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_LANGUAGE_DELEGATE_H
#define INSTALLER_UI_DELEGATES_LANGUAGE_DELEGATE_H

#include <QString>

namespace installer {

// Get filepath of default locale.
QString GetDefaultLocalePath();

// Get filepath of |locale|.
QString GetLocalePath(const QString& locale);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_LANGUAGE_DELEGATE_H
