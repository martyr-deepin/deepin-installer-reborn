// Copyright (c) 2015 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#ifndef INSTALLER_THIRD_PARTY_KEYBOARD_EVENT_FILTER_H_
#define INSTALLER_THIRD_PARTY_KEYBOARD_EVENT_FILTER_H_

#include <QAbstractNativeEventFilter>
#include <QHash>
#include <QPair>

#include "global_shortcut.h"

// Filter global keyboard event.
// This class is only used in GlobalShortcut.
class KeyboardEventFilter : public QAbstractNativeEventFilter {
 public:
  KeyboardEventFilter();
  ~KeyboardEventFilter();

  friend class GlobalShortcut;

  bool nativeEventFilter(const QByteArray& eventType, void* message,
                         long* result) override;


 private:
  bool registerShortcut(quint32 nativeKey, quint32 nativeMods);

  bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);

  QHash<QPair<quint32, quint32>, GlobalShortcut*> shortcuts_;
};

#endif  // INSTALLER_THIRD_PARTY_KEYBOARD_EVENT_FILTER_H_
