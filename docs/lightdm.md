# 关于
这里介绍一下如何用 lightdm 来启动安装器的进程, 并拥有 root 权限.

目前有两种方式可以实现:

## 第一种方式
先生成 lightdm.conf,

```shell
# cat > /etc/lightdm/lightdm.conf << EOF
[LightDM]
greeter-user=root

[Seat:*]
greeter-session=deepin-installer-greeter
EOF

```

再生成 deepin-installer-greeter.desktop
```shell
# cat > /usr/share/xgreeters/deepin-installer-greeter.desktop << EOF
[Desktop Entry]
Name=LightDM GTK+ Greeter
Comment=This runs the GTK+ greeter, it should only be run from LightDM
Exec=deepin-installer
Type=Application
X-Ubuntu-Gettext-Domain=lightdm
EOF
```

## 另一种方式
只需要生成 lightdm.conf
```shell
# cat > /etc/lightdm/lightdm.conf << EOF
greeter-setup-script=/usr/bin/deepin-installer
EOF
```
因为 lightdm 会以 root 的身份运行 greeter-setup-script 指向的脚本.
