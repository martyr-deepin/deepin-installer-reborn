#!/bin/bash

# SI = System Integrity

. "/deepin-installer/basic_utils.sh"
. "/usr/share/deepin-installer/hooks/basic_utils.sh"

if [ -d "/deepin-installer/before_check/" ];then
    for i in /deepin-installer/before_check/*; do
        chmod +x $i
        $i
        if [ $? != 0 ]; then
            echo "Check Mode faild: $i" >> /var/log/deepin-installer.log
            touch /boot/efi/SI_FAILED
            setNetworkBoot
            reboot
        fi
    done
fi

# wait for lightdm
sleep 5
