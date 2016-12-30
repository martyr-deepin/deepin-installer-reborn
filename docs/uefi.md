# 关于
这里介绍一些与UEFI, 签名相关的信息

## 生成 grub EFI 文件
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