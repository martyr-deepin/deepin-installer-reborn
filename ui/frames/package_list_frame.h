// Copyright (c) 2016 LiuLang. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
#define INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_

#include <QFrame>
class QStackedLayout;

namespace installer {

class SelectKernelFrame;
class ServicesPackageFrame;

// To display packages to be installed into target system.
class PackageListFrame : public QFrame {
  Q_OBJECT

 public:
  explicit PackageListFrame(QWidget* parent = nullptr);

 signals:
  void finished();

 private:
  void initConnections();
  void initUI();

  SelectKernelFrame* select_kernel_frame_ = nullptr;
  ServicesPackageFrame* services_package_frame_ = nullptr;
  QStackedLayout* stacked_layout_ = nullptr;

 private slots:
  void onSelectKernelFrameFinished();

  // Show SelectKernelFrame.
  void onServicesPackageFrameGoPrevious();
};

}  // namespace installer

#endif  // INSTALLER_UI_FRAMES_PACKAGE_LIST_FRAME_H_
