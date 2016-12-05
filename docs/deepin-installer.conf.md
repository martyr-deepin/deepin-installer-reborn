# 安装器生成的配置信息最终都会写入到 /etc/deepin-installer.conf.
# hooks 脚本会根据需要, 读取这里面的信息. 为了方便脚本来读取, deepin-installer.conf 支持
# 直接载入到脚本 `. /etc/deepin-installer.conf`


### 系统/用户信息
DI_SKIP_USER_INFO=0
# 下次启动时, 再设置用户信息, 可以为空.

DI_USERNAME=""
# 用于设置新建用户的用户名, 可以为空.

DI_PASSWORD=""
# base64编码过的用户密码, 可以为空.

DI_HOSTNAME=""
# 主机的hostname, 可以为空.

DI_AVATAR=""
# 用户头像, 可以为空.

DI_TIMEZONE=""
# 系统时区, 可以为空.

DI_LOCALE=""
# 系统语言, 可以为空.

DI_KEYBOARD_LAYOUT=""
# 键盘布局, 可以为空.

DI_KEYBOARD_LAYOUT_VARIANT=""
# 键盘布局变种, 可以为空.

DI_KEYBOARD_MODEL=""
# 设置键盘 model, 可以为空.

DI_KEYBOARD_OPTIONS=""
# 设置键盘xkboption, 可以为空.

DI_CUR_RESOLUTION=""
# 当前系统的分辨率

DI_VENDOR_NAME=""
# 公司/组织机构名称, 可以为空.

DI_OS_NAME=""
# 系统名称, 可以为空.

DI_OS_VERSION=""
# 系统版本号, 可以为空.


#### 分区信息
DI_DEFAULT_FS="ext4"
# 默认分区类型, 不可以为空.

DI_BOOTLOADER=""
# 引导器位置, 可以为空. 如果为空, 就不会安装grub.
# 如果是 EFI 引导模式, 这个选项的值应该是 EFI 分区的路径

DI_UEFI=""
# 是否支持EFI, "true" 或者 "false", 可以为空.

#DI_HOST_DEV=""

DI_ROOT_PARTITION=""
# 系统根分区, 不可以为空.

DI_ROOT_FSTYPE=""
# 系统根分区的分区类型.

DI_MOUNTPOINTS=""
# 系统的挂载点信息, 可以为空.
