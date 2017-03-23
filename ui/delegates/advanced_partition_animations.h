// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H

#include <QObject>

namespace installer {

class AdvancedPartitionButton;

// All animations of AdvancedPartitionFrame are defined and implemented in
// this class.
class AdvancedPartitionAnimations : public QObject {
  Q_OBJECT

 public:
  explicit AdvancedPartitionAnimations(QObject* parent = nullptr);

 public slots:
  // Show |widget| with animations. And |widget| shall be placed in a
  // QVBoxLayout.
  //  * opacity: 0% -> 100%
  //  * height: 80% -> 100%
  void showWidget(QWidget* widget);

  // Hide |widget| with animations. And |widget| shall be placed in a
  // QVBoxLayout.
  //  * opacity: 100% -> 0%
  //  * height: 100% -> 80%
  void hideWidget(QWidget* widget);

  // Set opacity of partition |button| to 20%.
  void highlightPartitionButton(AdvancedPartitionButton* button);
};

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H
