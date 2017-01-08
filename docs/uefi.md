# 关于
这里介绍一些与UEFI, 签名相关的信息

# 实现


## shim

### 生成deepin uefi ca

### 生成shimx64.efi
将 `deepin-uefi-ca.der` 放到 shim/debian, 然后编译shim项目

### 转换格式
将 shimx64.efi 转成cab格式: `lcab shimx64-unsigned.efi shimx64-unsigned.cab`

### 使用Verisign CA 为cab文件签名

### 上传签过名的cab文件到Microsoft sysdev

### 查看签名
使用命令 `pesign -n . -S -i shimx64-signed.efi` 来查看shim文件的签名情况.

当被Microsoft签名之后, 以上命令应该打印出类似的内容:

```text
certificate address is 0x7fecde3baa18
Content was not encrypted.
Content is detached; signature cannot be verified.
The signer's common name is Microsoft Windows UEFI Driver Publisher
No signer email address.
No signing time included.
There were certs or crls included.
```

## grub

### 生成 grub EFI 文件
使用以下命令, 可以生成定制的 grub EFI 文件
```bash
grub-mkimage -o bootx64.efi -O x86_64-efi -p /EFI/deepin ntfs hfs appleldr \
  boot cat efi_gop efi_uga elf fat hfsplus iso9660 linux keylayouts memdisk \
  minicmd part_apple ext2 extcmd xfs xnu part_bsd part_gpt search \
  search_fs_file chain btrfs loadbios loadenv lvm minix minix2 reiserfs \
  memrw mmap msdospart scsi loopback normal configfile gzio all_video efi_gop \
  efi_uga gfxterm gettext echo boot chain eval ls test sleep png gfxmenu
```
其中, -p /EFI/deepin 指定了签名文件的目录路径为 /EFI/deepin.

### 为 grubx64.efi 签名


# Tips and Issues
因为有些主板的EFI功能实现有的bug, 它可能会忽略写入到芯片中的EFI引导项, 而只是
读取特定路径里的efi模块, 比如fallback路径`EFI/BOOT/bootx64.efi`, 或者Windows
系统的启动路径`EFI/Microsoft/Boot/bootmgfw.efi`. 为了兼容这些有问题的主板, 我们
也可以复制一份`shimx64.efi`到`EFI/BOOT/bootx64.efi`.
