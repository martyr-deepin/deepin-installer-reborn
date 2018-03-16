#!/bin/sh
# Unsquashfs file and extract its progress value

SQUASHFS_FILE=$1

# Files to keep progress value.
BASE_PROGRESS_FILE=/dev/shm/unsquashfs_base_progress

# Get number of processors available for current process, minus 1.
PROCESSORS=$(getconf _NPROCESSORS_ONLN)
if [ $PROCESSORS -ge 1 ]; then
  PROCESSORS=$((PROCESSORS - 1))
else
  PROCESSORS=1
fi

deepin-installer-unsquashfs -p ${PROCESSORS} -f ${SQUASHFS_FILE}

#script -c "unsquashfs -p ${PROCESSORS} -f ${SQUASHFS_FILE} | deepin-installer-unsquashfs-progress ${BASE_PROGRESS_FILE}" /dev/null

#stty columns 80 ocrnl
#unsquashfs -p ${PROCESSORS} -f ${SQUASHFS_FILE}
#script -c "unsquashfs -p ${PROCESSORS} -f ${SQUASHFS_FILE} | deepin-installer-unsquashfs-progress" /dev/null
#unsquashfs -p ${PROCESSORS} -f ${SQUASHFS_FILE} | deepin-installer-unsquashfs-progress ${BASE_PROGRESS_FILE}

exit 0
