# 安装器自定义
这篇文档记录了安装器重制版支持的自定义功能.

在不修改源码的情况下, 所有的自定义文件, 都应该放在 oem 目录. 分发时， oem 目录应该
放在ISO的根目录， ISO应该挂载于 /lib/live/mount/medium/ 或者 /cdrom . 测试时,
为了方便, 可以将 oem 目录放在 /tmp 目录里. 它们的优先级从高到低依次是, /tmp/oem,
/cdrom/oem, /lib/live/mount/medium/oem

# 审核模式
审核模式会在启用重启后配置阶段进入一次桌面，并在lightdm启动前通过root权限执
行check_hooks/before_check/内的脚本，在lightdm关闭后执行check_hooks/after_check/的脚本，
在登录进桌面后执行check_hooks/in_check/里的脚本。由于审核模式的操作比较特殊，无法和普通的hooks定制
一起使用，在oem目录需要单独创建一个check_hooks的目录，内部结构为before_check、in_check和after_check三个目录。
如果某个脚本执行失败，会在esp中创建SI_FAILED，全部执行完毕，lightdm_stop.sh会在esp创建SI_SUCCESS。(SI = System Integrity)，并设置下一次启动网络启动，由PE进行检查。
* `DI_SI_USER` 定制审核模式的测试用户，默认为test。
* `DI_SI_PASSWORD` 定制审核模式测试用户的密码，默认为test。
* `DI_SI_GROUP` 定制审核模式测试用户的组，默认sudo。

# 自定义功能
安装器默认的配置信息集成到了程序里面, 在 `resources/default_settings.ini`,
自定义的话, 需要创建 `oem/settings.ini` 文件, 然后对选项重新赋值即可.
所有可用的配置选项都在 `default_settings.ini` 里面有说明.

## 各类图片
* 安装器背景. 放到 `oem/installer-background.jpg`. 如果需要模糊效果的话,
 可以用 `convert input.jpg -blur 0x32 installer-background.jpg`
 命令来生成模糊图片. 如果 ISO中 集成的系统默认壁纸发生了变化, 也需要在这里更新
 安装器的背景图.
* 桌面背景. 放到 `oem/background.jpg`.
* Logo. 在语言选择页面的正上方, 会显示系统 logo, 默认是 deepin 的官方logo,
 如果需要自定义, 可以放到 `oem/vendor.png`.
* 应用图标. `oem/icons/` 目录用于存放应用图标, 其目录结构应与
 `/usr/share/icons/hicolor` 相同.
* GRUB引导画面背景. 可以把图片放到 `oem/grub.jpg`.
* 登录界面背景. 可以放到 `oem/lightdm.png`
* plymouth 开机动画是一系统图片组成的, 放到 `oem/plymouth-theme/deepin-logo/`
 目录中. 可以参考 `plymouth-theme-deepin-logo` 这个deb包的结构.
* deepin 控制中心和桌面登录界面中用到的logo, 可以放到 oem/vendor.png
* 光盘启动动画. 待补充.

## 安装过程中的幻灯片展示
* "install_progress_page_disable_slide", 启用/禁用幻灯片切换.
* "install_progress_page_disable_slide_animation", 启用/禁用幻灯片切换时的过渡动画.
* "install_progress_page_animation_duration", 定义幻灯片切换时间间隔, 默认是 8000 毫秒.

如果需要定制幻灯片动画, 只需要把 slide 目录, 放置到 ISO_ROOT/oem/ 目录里面就可以了. 其中,
slide 目录的结果应该与 resources/slide 目录相同.
比如, 简体中文界面, 幻灯片的查找顺序依次是:
* ISO_ROOT/oem/slide/zh_CN
* /usr/share/deepin-installer/resources/slide/zh_CN
* ISO_ROOT/oem/slide/default
* /usr/share/deepin-installer/resources/slide/default

## 任务栏默认图标和顺序
在配置文件里面修改 `launcher_apps_in_launcher` 即可.

## 自动分区
需要把 `skip_partition_page` 设置为`1`, 然后把 `partition_do_auto_part`
设置为 `1`. 安装器自带的自动分区的脚本位于 `hooks/auto_part.sh`, 也可以自定义
自动分区脚本, 把需要分区的操作写到 `oem/auto_part.sh` 这个脚本文件里.
分区操作使用 `parted` 命令, 同时, 记得把分区结果写入到 `/etc/deepin-installer.conf`
这个配置文件里.

## 全盘分区
使用全盘分区时, 可以设定相应的分区策略:
* `partition_full_disk_large_disk_threshold`, 单位是Gib, 当选中的硬盘大小小于这个值时,
就认为它是小硬盘, 使用小硬盘的策略. 否则就使用大硬盘策略. 默认是 64Gib
* `partition_full_disk_small_legacy_policy`, 以 legacy 模式启动, 小硬盘策略
* `partition_full_disk_small_uefi_policy`, 以 EFI 方式启动, 小硬盘策略
* `partition_full_disk_large_legacy_policy`, 以 legacy 方式启动, 大硬盘策略
* `partition_full_disk_large_uefi_policy`, 以 EFI 方式启动, 大硬盘策略
* `partition_full_disk_large_root_part_range`, 大硬盘模式下, 根分区的大小需要在这个
范围之内, 单位是 Gib, 默认是 "20:150", 即 20Gib 到 150 Gib.

## hooks
根据安装阶段的不同, 分为 `before_chroot`, `in_chroot` 以及 `after_chroot`.
在 `oem/hooks/` 目录里, 创建相应的目录, 并写需要的hook脚本即可.

可以参考安装器 `hooks/` 目录里的脚本的写法.

同一个阶段的 hooks 脚本是按照文件名的先后来排序的. 只有以 `.job` 结尾的脚本才被认为
是合法的 hook 脚本.

在 `oem/hooks/` 目录里的脚本可以覆盖安装器自带的同名的脚本.

## deb包
* 需要额外安装的deb包, 都应该放到 `oem/deb/` 目录里, 并且, 它们的依赖关系应该能被
 自动满足, 否则可能无法正常安装.
* 卸载deb包, 可以在 `package_uninstall_packages` 里面设置.

## APT sources.list
* 需要覆盖默认软件包源，可以覆盖`apt_source_deb`的值
* 需要覆盖默认源码包源，可以覆盖`apt_source_deb_src`的值

## Grub
* 设置 `grub_timeout` 可以设定grub菜单的超时.
* `grub_block_windows_item` 可以用来屏蔽 windows 启动项.


## 系统基本信息
* `system_info_deepin_version` 这一项保存了 `/etc/deepin-version` 的内容,
 系统更新时, 应该先读取这个项的值, 再写入到 `/etc/deepin-version` 中. 它的值
 是经过 base64 编码的. * `system_info_lsb_release` 这一项保存了
 `/etc/lsb-release` 的内容, 系统更新时, 应该先读取这个项的值,
 再写入到 `/etc/lsb-release` 中. 它的值是经过 base64 编码的.
* `system_info_vendor_name` 用于设定组织名称.
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
 `system_info_password_strong_check` (bool) 控制是否启用密码强度检查，控制中心将会保持同样的配置。
 最少8个字符和最大16个字符，至少包含大小写英文、数字和符号的任意两种组合。
 `system_info_use_windows_time` 当开启了这个选项时, 如果硬盘上已经有 windows
 分区了, 就 不在使用UTC时间, 而使用本地时间. 这个选项用于解决windows系统时间相差
 8小时的问题.
* 自定义保留的用户名. 被保留的用户名, 在 `resources/reserved_usernames` 中定义,
 也可以创建 `oem/reserved_usernames` 来自定义要保留的用户名. 被保留的用户名,
 指的是系统中已经存在或者极可能会被某些包创建的用户名.
* `skip_network_page` 跳过静态网络配置界面
* `skip_control_platform_page` 跳过深度管控平台地区设置界面
* `skip_select_language_page_on_first_boot` 跳过安装后配置的语言界面，会读取默认值
* `end_point_control_server_url` 深度管控平台地区设置界面默认服务器地址
* `end_point_control_lock_server` 设置深度管控平台地区设置界面默认服务器地址是否只读

## 桌面环境相关的
* `dde_dock_app_list` 在dock中显示哪些应用图标.
* `dde_launcher_app_list` 在launcher中显示的哪些应用图标.
* `dde_launcher_hold_packages` 在launcher中禁止卸载这些应用.
* `dde_desktop_app_list` 在桌面显示哪些图标或应用.
* `dde_sound_effect_switches` 用来控制桌面音效开关.

## Lightdm
* `lightdm_enable_auto_login` 允许用户自动登录

## 服务器版自动激活
可以把生成的 license.key 放到 oem/ 目录里, 在安装时就会使用这个 key 文件激活
服务器版的系统.
