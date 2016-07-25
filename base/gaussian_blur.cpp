// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "base/gaussian_blur.h"

#include <QImage>

namespace base {

namespace {

}  // namespace

QPixmap FastGaussianBlur(const QString& image_path, int radius) {
  // TODO(xushaohua):
  Q_UNUSED(radius);
  return QPixmap(image_path);
}

}  // namespae base