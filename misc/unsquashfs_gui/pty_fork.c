
#define _XOPEN_SOURCE 600
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MAX_SNAME 1000

// See os.openpty() and os.forkpty() in Python's source repo.

// Open a pseudoterminal master and obtain the name of the corresponding
// pseudoterminal slave.
int pty_master_open(char* slave_name, size_t sn_len) {
  int master_fd;
  int last_errno;
  char* p;

  master_fd = posix_openpt(O_RDWR | O_NOCTTY);  // Opens pty master.
  if (master_fd == -1) {
    return -1;
  }

  if (grantpt(master_fd) == -1) {  // Grant access to slave pty.
    last_errno = errno;
    close(master_fd);
    errno = last_errno;
    return -1;
  }

  if (unlockpt(master_fd) == -1) {  // Unlock slave pty.
    last_errno = errno;
    close(master_fd);
    errno = last_errno;
    return -1;
  }

  p = ptsname(master_fd);  // Get slave pty name
  if (p == NULL) {
    last_errno = errno;
    close(master_fd);
    errno = last_errno;
    return -1;
  }

  if (strlen(p) < sn_len) {
    strncpy(slave_name, p, sn_len);
  } else {
    // Returns an error if buffer too small.
    close(master_fd);
    errno = EOVERFLOW;
    return -1;
  }

  return master_fd;
}

// Setting up a connection between child and parent process using
// a pseudoterminal pair and returns pid of child process.
pid_t pty_fork(int* master_fd, char* slave_name, size_t sn_len,
               const struct termios* slave_termios,
               const struct winsize* slave_ws) {
  int mfd, slave_fd, last_errno;
  pid_t child_pid;
  char slname[MAX_SNAME];

  mfd = pty_master_open(slname, MAX_SNAME);
  if (mfd == -1) {
    return -1;
  }

  if (slave_name != NULL) {  // Returns slave name to caller.
    if (strlen(slname) < sn_len) {
      strncpy(slave_name, slname, sn_len);
    } else {
      // slave_name was too small.
      close(mfd);
      errno = EOVERFLOW;
      return -1;
    }
  }

  child_pid = fork();

  if (child_pid == -1) {  // fork() failed.
    last_errno = errno;
    close(mfd);
    errno = last_errno;
    return -1;
  }

  if (child_pid != 0) {  // Parent process.
    *master_fd = mfd;
    return child_pid;
  }

  // Child process falls through to here.
  if (setsid() == -1) {  // Start a new session.
    return -1;
  }

  close(mfd);  // No need in child.

  slave_fd = open(slname, O_RDWR);  // Becomes controlling tty.
  if (slave_fd == -1) {
    return -1;
  }

  if (slave_termios != NULL) {  // Set slave tty attributes.
    if (tcsetattr(slave_fd, TCSANOW, slave_termios) == -1) {
      return -1;
    }
  }

  if (slave_ws != NULL) {  // Set slave tty window size.
    if (ioctl(slave_fd, TIOCSWINSZ, slave_ws) == -1) {
      return -1;
    }
  }

  // Duplicate pty slave to be child's stdin, stdout and stderr.
  if (dup2(slave_fd, STDIN_FILENO) != STDIN_FILENO) {
    return -1;
  }
  if (dup2(slave_fd, STDOUT_FILENO) != STDOUT_FILENO) {
    return -1;
  }
  if (dup2(slave_fd, STDERR_FILENO) != STDERR_FILENO) {
    return -1;
  }

  if (slave_fd > STDERR_FILENO) {  // Safety check.
    close(slave_fd);  // No longer need this fd.
  }

  return 0;
}

int main(void) {

}
