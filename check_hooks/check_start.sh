#!/bin/bash

. "/deepin-installer/basic_utils.sh"

# 这个文件是防止没有成功进到桌面，直接到lightdm_stop.sh了。
touch /tmp/in_check.file

if [ -d "/deepin-installer/in_check/" ];then
    for i in /deepin-installer/in_check/*; do
        chmod +x $i
        $i
        if [ $? != 0 ]; then
            echo ${DI_SI_PASSWORD}|sudo -S touch /boot/efi/SI_FAILED
            setNetworkBoot
            echo ${DI_SI_PASSWORD}|sudo reboot
        fi
    done
fi

# logout to run lightdm-stop.sh
qdbus --literal com.deepin.daemon.Display /com/deepin/SessionManager com.deepin.SessionManager.RequestLogout
