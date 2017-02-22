
# 关于
安装器生成的配置信息最终都会写入到 /etc/deepin-installer.conf. hooks 脚本会根据需要,
读取这里面的信息. 这里列出来的选项, 是安装器在运行时, 会动态生成的一些值. 其它的值都在
resources/default_settings.ini 文件里面有定义, 并且在 docs/oem.md 里面做了说明.
这里面列出来的一些选项, 可能会在系统安装完成并做新启动后, 才会被设置.


## 系统/用户信息
* DI_USERNAME, 用于设置新建用户的用户名, 可以为空.
* DI_PASSWORD, base64编码过的用户密码, 可以为空.
* DI_HOSTNAME, 主机的hostname, 可以为空.
* DI_AVATAR, 用户头像, 可以为空.
* DI_TIMEZONE, 系统时区, 可以为空.
* DI_LOCALE, 系统语言, 可以为空.
* DI_KEYBOARD_LAYOUT, 键盘布局, 可以为空.
* DI_KEYBOARD_LAYOUT_VARIANT, 键盘布局变种, 可以为空.
* DI_KEYBOARD_MODEL, 设置键盘 model, 可以为空.
* DI_KEYBOARD_OPTIONS, 设置键盘xkboption, 可以为空.
* DI_CUR_RESOLUTION, 当前系统的分辨率.

## 分区信息
* DI_BOOTLOADER, 引导器位置, 可以为空. 如果为空, 就不会安装grub. 如果是 EFI 引导模式,
 这个选项的值应该是 EFI 分区的路径
* DI_UEFI, 是否支持EFI, "true" 或者 "false", 不可以为空.
* DI_HOST_DEV,
* DI_ROOT_PARTITION, 系统根分区, 不可以为空.
* DI_ROOT_FSTYPE, 系统根分区的分区类型, 不可以为空.
* DI_MOUNTPOINTS, 系统的挂载点信息, 不可以为空.