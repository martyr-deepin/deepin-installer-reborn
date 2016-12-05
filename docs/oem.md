# 安装器自定义

## 关于
这篇文档记录了安装器重制版支持的自定义功能.

在不修改源码的情况下, 所有的自定义文件, 都应该放在 oem 目录.
oem 目录应该位于U盘(fat32格式)或系统光盘的根目录, 里面需要包含
`deepin-installer-reborn.lock` 文件, 它用于标记这是一个安装器的配置目录.
如果找到了多个 oem 目录, 会根据 `deepin-installer-reborn.lock` 这个文件的
新旧程度, 选择最新的那个, 其它的oem 目录均被忽略.

## 自定义功能
安装器默认的配置信息集成到了程序里面, 在 `resources/default_settings.ini`,
自定义的话, 需要创建 `oem/settings.ini` 文件, 然后对选项重新赋值即可.
所有可用的配置选项都在 `default_settings.ini` 里面有说明.

### 各类图片
* 安装器背景. 默认情况下, 会使用系统默认壁纸, 位于`/usr/share/backgrounds/default.jpg`.
 如果需要使用别的图片, 只需要放到 `oem/background.jpg`, 注意图片分辨率和大小. 如果
 需要模糊效果的话, 可以用 `convert input.jpg -blur 0x32 background.jpg`
 命令来生成模糊图片.
* Logo. 在语言选择页面的正上方, 会显示系统 logo, 默认是 deepin 的官方logo, 如果需要
 自定义, 可以放到 `oem/logo.png`.
* 应用图标. `oem/icons/` 目录用于存放应用图标, 其目录结构应与
 `/usr/share/icons/hicolor` 相同.
* 用户头像. 用户头像默认会使用 `dde-account-faces` 这个包提供的头像, 如果需要
 自定义, 可以把头像图片放到 `oem/avatar/` 目录里. 注意, 图片分辨率应该在
 `64x64`到`256x256`之间, 支持png和jpg格式. 如果要指定用户的默认头像, 需要
 设定 `system_info_default_avatar`.
* GRUB引导画面背景. 可以把图片放到 `oem/grub.png`.
* 登录界面背景. 可以放到 `oem/lightdm.png`
* plymouth 开机动画. 这个待补充.
* 光盘启动动画. 待补充.

### 任务栏默认图标和顺序
在配置文件里面修改 `launcher_apps_in_launcher` 即可.

### 自动分区
需要把 `skip_partition_page` 设置为`1`, 然后把 `partition_do_auto_part`
设置为 `1`. 安装器自带的自动分区的脚本位于 `hooks/auto_part.sh`, 也可以自定义
自动分区脚本, 把需要分区的操作写到 `oem/auto_part.sh` 这个脚本文件里.
分区操作使用 `parted` 命令, 同时, 记得把分区结果写入到 `/etc/deepin-installer.conf`
这个配置文件里. 要注意的是, `oem/auto_part.sh` 这个文件需要拥有可执行权限才行.

### 安装进程的动画
可以禁用动画, 通过设定 `install_progress_page_enable_slide`

### hooks
根据安装阶段的不同, 分为 `before_chroot`, `in_chroot` 以及 `after_chroot`.
在 `oem/hooks/` 目录里, 创建相应的目录, 并写需要的hook脚本即可.

可以参考安装器 `hooks/` 目录里的脚本的写法.

同一个阶段的 hooks 脚本是按照文件名的先后来排序的. 只有以 `.job` 结尾的脚本才被认为
是合法的 hook 脚本.

在 `oem/hooks/` 目录里的脚本可以覆盖安装器自带的同名的脚本.

### deb包
* 需要额外安装的deb包, 都应该放到 `oem/deb/` 目录里, 并且, 它们的依赖关系应该能被
 自动满足, 否则可能无法正常安装.
* 卸载deb包, 可以在 `package_uninstall_packages` 里面设置.
* 禁止卸载某些deb包, 可以在 `package_hold_packages` 里面设定.

### Grub
* 设置 `grub_timeout` 可以设定grub菜单的超时.
* `grub_block_windows_item` 可以用来屏蔽 windows 启动项.


### 系统基本信息
* `system_info_vendor_name` 用于设定组织名称.
* `system_info_os_name` 用于设定系统名称.
* `system_info_os_version` 设定系统版本号.
* `system_info_default_username` 设定默认用户名.
* `system_info_default_hostname` 默认主机名.
* `system_info_default_password` 默认密码.
* `system_info_setup_after_reboot` 用于跳过帐户设置, 在下次重启系统再设定.
* `skip_system_info_page` 跳过帐户设置页面, 但可以读取默认值.
* `system_info_password_min_len`, `system_info_password_max_len`,
 `system_info_password_require_number`,
 `system_info_password_require_lower_case`,
 `system_info_password_require_upper_case` 以及
 `system_info_password_require_special_char` 用于设定密码策略.