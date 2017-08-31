#!/usr/bin/env python
# -*- coding: utf-8 -*-

#smk is sw machine kernel file

import os
import sys
import subprocess
import json
import codecs

SW_ITMES = ["Linfa机型","Tembin机型"]

def file2smkjson(smkfile):
    with open(smkfile) as smkin:
        try:
            smk_json = json.load(smkin)
        except:
            return None
    return smk_json

def smkjson2file(smkfile,smkjson):
    os.remove(smkfile)
    with codecs.open(smkfile, "w", "utf-8") as smkout:
        json.dump(smkjson, smkout, sort_keys=True, ensure_ascii=False, indent=4)

def get_system_type():
    cmd = "/bin/grep ^'system type' /proc/cpuinfo | awk '{ print $NF }'"
    cpu_system_type = subprocess.check_output(cmd, shell=True).strip()
    return cpu_system_type

def get_select_index(system_type,items):
    #if system_type in "Linfa":
    for idex ,item in enumerate(items):
        if system_type in item:
            return idex 
    return None

def get_smkfile_path():
    smkfile = "select_machine_kernel.json"
    if os.path.exists(smkfile):
        return smkfile
    smkfile = "/usr/share/deepin-installer/resources/" + smkfile
    if os.path.exists(smkfile):
        return smkfile
    return None
            
def run_main():
    smkfile_path = get_smkfile_path()
    if not smkfile_path:
        print("not found smkfile!")
        sys.exit(1)

    smkjson = file2smkjson(smkfile_path)
    if not smkjson:
        print("read smkjson file error!")
        sys.exit(1)

    system_type = get_system_type()
    if not system_type:
        print("not found support cpu type!")
        sys.exit(1)

    smkjson["selected"] = get_select_index(system_type,smkjson["items"])

    smkjson2file(smkfile_path,smkjson)

if __name__ == "__main__":
    run_main()
