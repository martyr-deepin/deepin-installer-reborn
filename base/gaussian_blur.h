// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_BASE_GAUSSIAN_BLUR_H
#define DEEPIN_INSTALLER_REBORN_BASE_GAUSSIAN_BLUR_H

#include <QPixmap>

namespace base {

// Generate pix map object with blur effect based on
// Fast Gaussian Blur algorithm.
// Why not use qt_blurImage()? Well, it uses exponential-blur algorithm, but
// the implementations has some defect, such as white edges.
QPixmap FastGaussianBlur(const QString& image_path, int radius);

}  // namespace base

#endif  // DEEPIN_INSTALLER_REBORN_BASE_GAUSSIAN_BLUR_H
