// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/package_list_frame.h"

#include <QStackedLayout>

#include "ui/frames/inner/select_kernel_frame.h"
#include "ui/frames/inner/services_package_frame.h"

namespace installer {

PackageListFrame::PackageListFrame(QWidget* parent) : QFrame(parent) {
  this->setObjectName("package_list_frame");

  this->initUI();
  this->initConnections();
}

void PackageListFrame::initConnections() {
  connect(select_kernel_frame_, &SelectKernelFrame::finished,
          this, &PackageListFrame::onSelectKernelFrameFinished);
  connect(services_package_frame_, &ServicesPackageFrame::goPrevious,
          this, &PackageListFrame::onServicesPackageFrameGoPrevious);
  connect(services_package_frame_, &ServicesPackageFrame::finished,
          this, &PackageListFrame::finished);
}

void PackageListFrame::initUI() {
  select_kernel_frame_ = new SelectKernelFrame();
  services_package_frame_ = new ServicesPackageFrame();

  stacked_layout_ = new QStackedLayout();
  stacked_layout_->setContentsMargins(0, 0, 0, 0);
  stacked_layout_->setSpacing(0);
  stacked_layout_->addWidget(select_kernel_frame_);
  stacked_layout_->addWidget(services_package_frame_);

  this->setContentsMargins(0, 0, 0, 0);
  this->setLayout(stacked_layout_);
}

void PackageListFrame::onSelectKernelFrameFinished() {
  stacked_layout_->setCurrentWidget(services_package_frame_);
}

void PackageListFrame::onServicesPackageFrameGoPrevious() {
  stacked_layout_->setCurrentWidget(select_kernel_frame_);
}

}  // namespace installer