#!/bin/bash

# SI = System Integrity

. "/deepin-installer/basic_utils.sh"

if [ ! -f "/tmp/in_check.file" ];
then
    echo "Not enter in_check!!" >> /var/log/deepin-installer.log
    touch /boot/efi/SI_FAILED
    setNetworkBoot
    reboot
fi

if [ -d "/deepin-installer/after_check/" ];then
    for i in /deepin-installer/after_check/*; do
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

if [ ! -f "/boot/efi/SI_FAILED" ];then
    echo "Check Mode: Success!" >> /var/log/deepin-installer.log
    touch /boot/efi/SI_SUCCESS
    setNetworkBoot
    # remove check mode files and test user
    userdel -f ${DI_SI_USER}
    rm -rf /deepin-installer
    reboot
fi
