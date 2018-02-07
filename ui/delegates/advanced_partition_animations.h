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
