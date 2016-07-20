# 安装器自定义

## 关于
这篇文档记录了安装器重制版支持的自定义功能.
在不修改源码的情况下, 所有的自定义文件, 都应该放在 `ISO_ROOT/oem/` 目录.

## 自定义功能
安装器默认的配置信息集成到了程序里面, 在 `resources/default-settings.ini`,
自定义的话, 需要创建 `oem/settings.ini` 文件, 然后对选项重新赋值即可.
所有可用的配置选项都在 `default-settings.ini` 里面有说明.


### 安装器背景
默认情况下, 会使用系统默认壁纸, 位于`/usr/share/backgrounds/default.jpg`.
如果需要使用别的壁纸, 只需要放到 `oem/background.jpg` 即可.

注意图片分辨率和大小.


### Logo
在语言选择页面的正上方, 会显示系统 logo, 默认是 deepin 的官方logo, 如果需要
自定义, 可以放到 `oem/logo.png`.


### 用户头像
用户头像默认会使用 `dde-account-faces` 这个包提供的头像, 如果需要自定义, 可以把
头像图片放到 `oem/avatar/` 目录里.

注意, 图片分辨率应该在`64x64`到`256x256`之间, 支持png和jpg格式.

如果要指定用户的默认头像, 需要在 `oem/settings.ini` 文件里修改.


### hooks
根据安装阶段的不同, 分为 `before_chroot`, `in_chroot` 以及 `after_chroot`.
在 `oem/hooks/` 目录里, 创建相应的目录, 并写需要的hook脚本即可.

可以参考安装器 `hooks/` 目录里的脚本的写法.
