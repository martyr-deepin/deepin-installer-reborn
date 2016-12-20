// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

// Re-implement `timedatectl set-local-rtc` command.

#include <stdio.h>
#include <string.h>

#include <assert.h>
#include <fcntl.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "base/file_util.h"

const char kUsage[] = ""
    "Set local time or utc time\n"
    "Usage: %s 0|1\n"
    " 0 - Set as local time\n"
    " 1 - Set as UTC time\n";

// Write settings to /etc/adjtime.
// From timedated.c:context_write_data_local_rtc().
bool write_local_rtc(bool is_utc) {
  const char kAdjFile[] = "/etc/adjtime";
  QString content = installer::ReadFile(kAdjFile);
  if (is_utc) {
    content.replace("LOCAL", "UTC");
  } else {
    content.replace("UTC", "LOCAL");
  }

  return installer::WriteTextFile(kAdjFile, content);
}

int clock_get_hwclock(struct tm *tm) {
  int fd = -1;

  assert(tm);

  fd = open("/dev/rtc", O_RDONLY|O_CLOEXEC);
  if (fd < 0) {
    return 1;
  }

  /* This leaves the timezone fields of struct tm
   * uninitialized! */
  if (ioctl(fd, RTC_RD_TIME, tm) < 0) {
    close(fd);
    return 1;
  }

  /* We don't know daylight saving, so we reset this in order not
   * to confuse mktime(). */
  tm->tm_isdst = -1;

  close(fd);
  return 0;
}

// From systemd/src/basic/clock-utils.c
int clock_set_timezone() {
  const struct timeval *tv_null = NULL;
  struct timespec ts;
  struct tm *tm;
  int minutesdelta;
  struct timezone tz;

  if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
    return 1;
  }
  tm = localtime(&ts.tv_sec);
  assert(tm);

  minutesdelta = tm->tm_gmtoff / 60;

  tz.tz_minuteswest = -minutesdelta;
  tz.tz_dsttime = 0; /* DST_NONE */

  /*
   * If the RTC does not run in UTC but in local time, the very first
   * call to settimeofday() will set the kernel's timezone and will warp the
   * system clock, so that it runs in UTC instead of the local time we
   * have read from the RTC.
   */
  return (settimeofday(tv_null, &tz) == 0);
}

// From systemd/src/basic/clock-util.c
int clock_set_hwclock(const struct tm *tm) {
  assert(tm);
  int fd = -1;
  fd = open("/dev/rtc", O_RDONLY | O_CLOEXEC);
  if (fd < 0) {
    return 1;
  }
  if (ioctl(fd, RTC_SET_TIME, tm) < 0) {
    close(fd);
    return 2;
  }
  close(fd);
  return 0;
}

int main(int argc, char* argv[]) {
  if (getuid() != 0) {
    fprintf(stderr, "Root privilege is required!\n");
    return 1;
  }
  if (argc != 2) {
    fprintf(stderr, kUsage, argv[0]);
    return 1;
  }

  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
    fprintf(stderr, "clock_gettime() failed\n");
    return 1;
  }

  struct tm tm;
  const bool is_utc = (strncmp(argv[1], "0", 1) != 0);

  // 1. Write new configuration file
  if (!write_local_rtc(is_utc)) {
    fprintf(stderr, "Failed to update adjtime file\n");
    return 1;
  }

  // 2. Notify kernel about the timezone
  clock_set_timezone();

  // 3. Synchronize clocks.
  if (is_utc) {
    tm = *gmtime(&ts.tv_sec);
  } else {
    tm = *localtime(&ts.tv_sec);
  }

  return clock_set_hwclock(&tm);
}