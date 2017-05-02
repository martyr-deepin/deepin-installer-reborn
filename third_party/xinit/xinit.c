/*

Copyright 1986, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

#include "xinit.h"

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>

/* For PRIO_PROCESS and setpriority() */
#include <sys/time.h>
#include <sys/resource.h>

#include <stdlib.h>

#define TRUE 1
#define FALSE 0

static char *displayNum = ":0";
static char *default_server[] = {"X", "-nolisten", "tcp", "-noreset", "-br", ":0", NULL};

static char *default_client[] = {"deepin-installer-reborn", NULL};
static Display *xd = NULL;            /* server connection */
int status;
pid_t serverpid = -1;
pid_t clientpid = -1;
volatile int gotSignal = 0;

static void Execute(char **vec);
static Bool waitforserver(void);
static Bool processTimeout(int timeout, const char *string);
static pid_t startServer(char *server[]);
static pid_t startClient(char *client[]);
static int ignorexio(Display *dpy);
static void shutdown(void);
static void set_environment(void);

static void Fatal(const char *fmt, ...) _X_ATTRIBUTE_PRINTF(1,2) _X_NORETURN;
static void Error(const char *fmt, ...) _X_ATTRIBUTE_PRINTF(1,2);
static void Fatalx(const char *fmt, ...) _X_ATTRIBUTE_PRINTF(1,2) _X_NORETURN;
static void Errorx(const char *fmt, ...) _X_ATTRIBUTE_PRINTF(1,2);

static void
sigCatch(int sig)
{
    /* On system with POSIX signals, just interrupt the system call */
    gotSignal = sig;
}

static void
sigIgnore(int sig)
{
    (void) sig;
}

static void
Execute(char **vec)		/* has room from up above */
{
    execvp(vec[0], vec);
    return;
}

int
real_main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    pid_t pid;
    struct sigaction sa, si;
    /*
     * Start the server and client.
     */
    signal(SIGCHLD, SIG_DFL);    /* Insurance */

    /* Let those signal interrupt the wait() call in the main loop */
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sigCatch;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;    /* do not set SA_RESTART */

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);

    memset(&si, 0, sizeof(si));
    si.sa_handler = sigIgnore;
    sigemptyset(&si.sa_mask);
    si.sa_flags = SA_RESTART;

    sigaction(SIGALRM, &si, NULL);
    sigaction(SIGUSR1, &si, NULL);

    if (startServer(default_server) > 0
        && startClient(default_client) > 0) {
        pid = -1;
        while (pid != clientpid && pid != serverpid
               && gotSignal == 0
            )
            pid = wait(NULL);
    }

    signal(SIGTERM, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    shutdown();

    if (gotSignal != 0) {
        Errorx("unexpected signal %d", gotSignal);
        exit(EXIT_FAILURE);
    }

    if (serverpid < 0)
        Fatalx("server error");
    if (clientpid < 0)
        Fatalx("client error");
    exit(EXIT_SUCCESS);
}


/*
 *    waitforserver - wait for X server to start up
 */
static Bool
waitforserver(void)
{
    int    ncycles     = 120;        /* # of cycles to wait */
    int    cycles;            /* Wait cycle count */

    for (cycles = 0; cycles < ncycles; cycles++) {
        if ((xd = XOpenDisplay(displayNum))) {
            return(TRUE);
        }
        else {
            if (!processTimeout(1, "X server to begin accepting connections"))
              break;
        }
    }

    Errorx("giving up");

    return(FALSE);
}

/*
 * return TRUE if we timeout waiting for pid to exit, FALSE otherwise.
 */
static Bool
processTimeout(int timeout, const char *string)
{
    int    i = 0;
    pid_t  pidfound = -1;
    static const char    *laststring;

    for (;;) {
        if ((pidfound = waitpid(serverpid, &status, WNOHANG)) == serverpid)
            break;
        if (timeout) {
            if (i == 0 && string != laststring)
                fprintf(stderr, "\r\nwaiting for %s ", string);
            else
                fprintf(stderr, ".");
            fflush(stderr);
            sleep(1);
        }
        if (++i > timeout)
            break;
    }
    if (i > 0) fputc('\n', stderr);     /* tidy up after message */
    laststring = string;
    return (serverpid != pidfound);
}

static pid_t
startServer(char *server_argv[])
{
    sigset_t mask, old;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &old);

    serverpid = fork();

    switch(serverpid) {
    case 0:
        /* Unblock */
        sigprocmask(SIG_SETMASK, &old, NULL);

        /*
         * don't hang on read/write to control tty
         */
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        /*
         * ignore SIGUSR1 in child.  The server
         * will notice this and send SIGUSR1 back
         * at xinit when ready to accept connections
         */
        signal(SIGUSR1, SIG_IGN);
        /*
         * prevent server from getting sighup from vhangup()
         * if client is xterm -L
         */
        setpgid(0,getpid());
        Execute(server_argv);

        Error("unable to run server \"%s\"", server_argv[0]);

        exit(EXIT_FAILURE);

        break;
    case -1:
        break;
    default:
        /*
         * don't nice server
         */
        setpriority(PRIO_PROCESS, serverpid, -1);

        errno = 0;
        if(! processTimeout(0, "")) {
            serverpid = -1;
            break;
        }
        /*
         * kludge to avoid race with TCP, giving server time to
         * set his socket options before we try to open it,
         * either use the 15 second timeout, or await SIGUSR1.
         *
         * If your machine is substantially slower than 15 seconds,
         * you can easily adjust this value.
         */
        alarm(15);

        sigsuspend(&old);
        alarm(0);
        sigprocmask(SIG_SETMASK, &old, NULL);

        if (waitforserver() == 0) {
            Error("unable to connect to X server");
            shutdown();
            serverpid = -1;
        }
        break;
    }

    return(serverpid);
}

static void
setWindowPath(void)
{
    /* setting WINDOWPATH for clients */
    Atom prop;
    Atom actualtype;
    int actualformat;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *buf;
    const char *windowpath;
    char *newwindowpath;
    unsigned long num;
    char nums[10];
    int numn;
    size_t len;
    prop = XInternAtom(xd, "XFree86_VT", False);
    if (prop == None) {
        Errorx("Unable to intern XFree86_VT atom");
        return;
    }
    if (XGetWindowProperty(xd, DefaultRootWindow(xd), prop, 0, 1,
        False, AnyPropertyType, &actualtype, &actualformat,
        &nitems, &bytes_after, &buf)) {
        Errorx("No XFree86_VT property detected on X server, WINDOWPATH won't be set");
        return;
    }
    if (nitems != 1) {
        Errorx("XFree86_VT property unexpectedly has %lu items instead of 1", nitems);
        XFree(buf);
        return;
    }
    switch (actualtype) {
    case XA_CARDINAL:
    case XA_INTEGER:
    case XA_WINDOW:
        switch (actualformat) {
        case  8:
            num = (*(uint8_t  *)(void *)buf);
            break;
        case 16:
            num = (*(uint16_t *)(void *)buf);
            break;
        case 32:
            num = (*(uint32_t *)(void *)buf);
            break;
        default:
            Errorx("XFree86_VT property has unexpected format %d", actualformat);
            XFree(buf);
            return;
        }
        break;
    default:
        Errorx("XFree86_VT property has unexpected type %lx", actualtype);
        XFree(buf);
        return;
    }
    XFree(buf);
    windowpath = getenv("WINDOWPATH");
    numn = snprintf(nums, sizeof(nums), "%lu", num);
    if (!windowpath) {
        len = numn + 1;
        newwindowpath = malloc(len);
        if (newwindowpath == NULL)
            return;
        snprintf(newwindowpath, len, "%s", nums);
    } else {
        len = strlen(windowpath) + 1 + numn + 1;
        newwindowpath = malloc(len);
        if (newwindowpath == NULL)
            return;
        snprintf(newwindowpath, len, "%s:%s",
                 windowpath, nums);
    }
    if (setenv("WINDOWPATH", newwindowpath, TRUE) == -1)
        Error("unable to set WINDOWPATH");


    free(newwindowpath);
}

static pid_t
startClient(char *client_argv[])
{
    clientpid = fork();
    if (clientpid == 0) {
        set_environment();
        setWindowPath();

        if (setuid(getuid()) == -1) {
            Error("cannot change uid");
            _exit(EXIT_FAILURE);
        }
        setpgid(0, getpid());
        Execute(client_argv);
        Error("Unable to run program \"%s\"", client_argv[0]);
        _exit(EXIT_FAILURE);
    } else {
        return clientpid;
    }
}

static jmp_buf close_env;

static int
ignorexio(Display *dpy)
{
    (void) dpy;
    Errorx("connection to X server lost");
    longjmp(close_env, 1);
    /*NOTREACHED*/
    return 0;
}

static void
shutdown(void)
{
    /* have kept display opened, so close it now */
    if (clientpid > 0) {
        XSetIOErrorHandler(ignorexio);
        if (! setjmp(close_env)) {
            XCloseDisplay(xd);
        }

        /* HUP all local clients to allow them to clean up */
        if (killpg(clientpid, SIGHUP) < 0 && errno != ESRCH)
            Error("can't send HUP to process group %d", clientpid);
    }

    if (serverpid < 0)
        return;

    if (killpg(serverpid, SIGTERM) < 0) {
        if (errno == ESRCH)
            return;
        Fatal("can't kill X server");
    }

    if (!processTimeout(10, "X server to shut down"))
        return;

    Errorx("X server slow to shut down, sending KILL signal");

    if (killpg(serverpid, SIGKILL) < 0) {
        if (errno == ESRCH)
            return;
        Error("can't SIGKILL X server");
    }

    if (processTimeout(3, "server to die"))
        Fatalx("X server refuses to die");
}

static void
set_environment(void)
{
    if (setenv("DISPLAY", displayNum, TRUE) == -1)
        Fatal("unable to set DISPLAY");
}

static void _X_ATTRIBUTE_PRINTF(1,0)
verror(const char *fmt, va_list ap)
{
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ": %s\n", strerror(errno));
}

static void _X_ATTRIBUTE_PRINTF(1,0)
verrorx(const char *fmt, va_list ap)
{
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

static void
Fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror(fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

static void
Fatalx(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrorx(fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

static void
Error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror(fmt, ap);
    va_end(ap);
}

static void
Errorx(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verrorx(fmt, ap);
    va_end(ap);
}
