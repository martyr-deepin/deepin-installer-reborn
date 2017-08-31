#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
smk is sw machine kernel
ssp is sw services packages
"""
PKG_INSTALL_TPL="""#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# cat <<EOF > /etc/apt/sources.list.d/00-cdrom.list
# # sw repo in cdrom
# deb file:///${CDROM}/sw64 kui main
# EOF

export DEBIAN_FRONTEND=noninteractive

# Update CDROM Repo Metadata
apt-get update -q

# Install SW pkg list
apt-get install -q -y --force-yes --allow-unauthenticated -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" -o Dpkg::Options::="--force-overwrite" %s

rm -f /etc/apt/sources.list.d/00-cdrom.list

exit 0
"""

import os
import sys
import json

DEEPIN_INSTALL_PATH="/usr/share/deepin-installer"

def print_help():
    print("%s : [smk/ssp] argv"%sys.argv[0])

def file2json_data(sfile):
    sfile = DEEPIN_INSTALL_PATH +"/resources/" + sfile
    print(sfile)
    if os.path.exists(sfile):
        with open(sfile) as json_input:
            try:
                json_data = json.load(json_input,encoding="utf-8")
                return json_data
            except:
                return None
    return None

def buffer2list():
    input_list = (unicode(sys.argv[2], "utf-8")).split(";")
    return map(lambda x: x.lower(), input_list)

"""
sw64 select dialog selected machine kernel method
"""
def smk2sh():
    print("smk")
    smk_json = file2json_data("select_machine_kernel.json")
    #print smk_json
    pass

"""
sw64 select dialog selected service package method
"""
def ssp2sh():
    #print("ssp")
    select_pkg_list = []

    ssp_json = file2json_data("select_services_packages.json")

    ssp_select_list = buffer2list()

    for ssp_item in ssp_select_list:
        if ssp_item in ssp_json:
            select_pkg_list.append(" ".join(ssp_json[ssp_item]))

    ssp_pkg_install_list = " ".join(select_pkg_list)

    ssp_sh_job = DEEPIN_INSTALL_PATH + "/hooks/in_chroot/21_sw_install_select_pkg.job"

    with open(ssp_sh_job,"w+") as ssp_sh_output:
        ssp_sh_output.write(PKG_INSTALL_TPL%(ssp_pkg_install_list))

    if os.path.exists(ssp_sh_job):
        os.system("chmod +x %s"%ssp_sh_job)
        ret = os.system("/bin/bash %s"%ssp_sh_job)
        print("run %s ret [%d]"%(ssp_sh_job,ret/255))
        os.remove(ssp_sh_job)

def run_main():
    if len(sys.argv) is 3:
        #print(sys.argv)
        if sys.argv[1] in "smk":
            smk2sh()
        elif sys.argv[1] in "ssp":
            ssp2sh()
        else:
            print_help()
    else:
        print_help()


if __name__ == "__main__":
    run_main()
    sys.exit(0)

