## 前端页面的基本结构如下:

```
MainWindow
  /-- BackgroundLabel
  |-- CloseButton
  |-- PageIndicator
  |-- ConfirmQuitFrame
  |-- ControlPanelFrame
  \-- StackedLayout
    /-- PrivilegeErrorFrame
    |-- DiskSpaceInsufficientFrame
    |-- VirtualMachineFrame
    |-- SelectLanguageFrame
    |-- SystemInfoFrame
      /-- FormFrame
      |-- AvatarFrame
      \-- KeyboardLayoutFrame
    |-- TimezoneFrame
    |-- PartitionFrame
      /-- PartitionLoadingFrame
      |-- SimpleDiskFrame
      |-- SimplePartitionFrame
      |-- AdvancedPartitionFrame
      |-- PartitionTableWarningFrame
      |-- EditPartitionFrame
      |-- NewPartitionFrame
      |-- NewTableLoadingFrame
      |-- PartitionNumberLimitationFrame
      |-- SelectBootLoaderFrame
      \-- PrepareInstallFrame
    |-- InstallProgressFrame
      +-- InstallProgressSlideFrame
    |-- InstallSuccessFrame
    \-- InstallFailedFrame
```
