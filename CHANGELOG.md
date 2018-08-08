<a name=""></a>
##  2.3.0 (2018-08-08)


#### Bug Fixes

* **FullInstall:**  root partition offset error ([69bf3462](https://github.com/linuxdeepin/deepin-install-reborn/commit/69bf3462172fcd8efff481213352717410aab6cf))



<a name="2.2.19"></a>
### 2.2.19 (2018-08-07)


#### Bug Fixes

*   precision problem when calculating swap ([c4fded0f](https://github.com/linuxdeepin/deepin-install-reborn/commit/c4fded0ff4d2a2ba0177590d67ce51928fd8a07b))
*   wrong tip width ([1bc3da24](https://github.com/linuxdeepin/deepin-install-reborn/commit/1bc3da2484840137ee37993e37f3eb0cd9eb9071))
*   unable to display partition number correctly ([ed501c90](https://github.com/linuxdeepin/deepin-install-reborn/commit/ed501c90ebc089bf86d7e20ac0e32649d67bc8ab))

#### Features

*   support disk label for full disk install mode ([5def6bad](https://github.com/linuxdeepin/deepin-install-reborn/commit/5def6bad9a021eeb795dbeb803c57e02bb885a32))
*   support auto swap ([b48f4b8b](https://github.com/linuxdeepin/deepin-install-reborn/commit/b48f4b8bcdae27f17cc72eb9fbbd6cf1740a3e53))
*   modify the order of installation ([f4b549da](https://github.com/linuxdeepin/deepin-install-reborn/commit/f4b549da49eec1a78607f95874933b82f903b46e))
* **disk:**  change simple mode to first ([a0e9689f](https://github.com/linuxdeepin/deepin-install-reborn/commit/a0e9689fc7d8a545adde4c975c267fa0b8ef9530))


2018-07-31 2.2.18 haruyukilxz <lxz@ilxz.me>
#### Bug Fixes

*   no grub item of live-filesystem on sw ([707fa0b9](707fa0b9))
*   missing live-fileysystem uuid on sw ([8c014f8e](8c014f8e))
*   Adapt lintian ([7f04a6c2](7f04a6c2))
* **tip:**  wrong hints are not newline ([81384c5d](81384c5d))

#### Features

*   set default password for livefs. ([235be0cc](235be0cc))
*   add arm64-efi support ([097258d4](097258d4))
*   add cryptroot support ([ec8d136e](ec8d136e))

2018-06-07 2.2.17 Xu Shaohua <xushaohua2016@outlook.com>
    * Add screen brightness shortcut
    * Support empty password
    * Hide timezone page if it is disabled

2018-05-28 2.2.16.1 deepinzhangshuang <zhangshuang@deepin.com>
    * fix /etc/default/grub GRUB_DISTRIBUTOR
    * Add live-filesystem grub menu on sw
    * fix symlinks in /boot/boot on sw

2018-05-14 2.2.16 Xu Shaohua <xushaohua2016@outlook.com>
    * Fix live-filesystem config for sw and loongson
    * Set sw421 sources.list to as4s

2018-03-11 2.2.15.3 Xu Shaohua <xushaohua@deepin.com>
    * Check existence of /etc/deepin-version

2018-04-10 2.2.15.2 Xu Shaohua <xushaohua@deepin.com>
    * Fix timestamp issue of APT cache packages
    * Add installer user form
    * Add resolution settings for grub on arm64

2018-03-16 2.2.15.1 Xu Shaohua <xushaohua@deepin.com>
    * Set password for live environment

2018-03-15 2.2.15 Xu Shaohua <xushaohua@deepin.com>
    * Add arm64 source file list
    * Add settings for professional edition
    * Fix shell script syntax error
    * Update i18n

2018-01-25 2.2.14 Xu Shaohua <xushaohua@deepin.com>
	* Support arm64

2017-11-29 2.2.13 Xu Shaohua <xushaohua@deepin.com>
	* Update grub based on current locale

2017-11-29 2.2.12 Xu Shaohua <xushaohua@deepin.com>
	* Uninstall live-filesystem in lupin mode

2017-11-28 2.2.11 Xu Shaohua <xushaohua@deepin.com>
	* Remove live-filesystem package for lupin

2017-11-22 2.2.10 Xu Shaohua <xushaohua@deepin.com>
	* Generate font cache for wine QQ

2017-11-22 2.2.9 Xu Shaohua <xushaohua@deepin.com>
	* Remove target character file before calling mknod()

2017-11-20 2.2.8 Xu Shaohua <xushaohua@deepin.com>
	* 更新翻译
	* 修改自动分区时, boot 分区最小容量为 500M
	* 分区时, 只要在必要时再遍历home目录修改属性

2017-11-07 2.2.7 Xu Shaohua <xushaohua@deepin.com>
	* installer-setting 支持保存 StringList
	* 过滤 dde_launcher_hold_packages

2017-11-07 2.2.6 Xu Shaohua <xushaohua@deepin.com>
	* 更新翻译
	* 支持批量激活
	* 修正软件包依赖关系
	* 修正用户名错误提示
	* 提示用户弹出安装设备
