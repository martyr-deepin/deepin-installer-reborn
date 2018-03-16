#!/usr/bin/env python3

import argparse
import os
import pty
import sys
import time

filename = "/tmp/typescript"

with open(filename, 'wb') as script:
    def read(fd):
        data = os.read(fd, 1024)
        script.write(data)
        return data

    print('Script started, file is', filename)
    script.write(('Script started on %s\n' % time.asctime()).encode())

    pty.spawn("./unsquashfs.sh", read)

    script.write(('Script done on %s\n' % time.asctime()).encode())
    print('Script done, file is', filename)
