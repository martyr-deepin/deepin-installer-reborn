// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H
#define INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H

#include <QObject>

namespace installer {

class AdvancedPartitionButton;

// All animations of AdvancedPartitionFrame are defined and implemented here.

// Hide |widget| with animations. And |widget| shall be placed in a
// QVBoxLayout.
//  * opacity: 100% -> 0%
//  * height: 100% -> 80%
void AnimationHideWidget(QWidget* widget);

// Show |widget| with animations. And |widget| shall be placed in a
// QVBoxLayout.
//  * opacity: 0% -> 100%
//  * height: 80% -> 100%
void AnimationShowWidget(QWidget* widget);

// Set opacity of partition |button| to 20%.
void AnimationHighlightPartitionButton(AdvancedPartitionButton* button);

}  // namespace installer

#endif  // INSTALLER_UI_DELEGATES_ADVANCED_PARTITION_ANIMATIONS_H
