// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by Commercial License that can be found
// in the LICENSE file.

#include "ui/utils/xutil.h"

#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/XTest.h>

// From the WM spec
#define _NET_WM_MOVERESIZE_INVALID          -1
#define _NET_WM_MOVERESIZE_SIZE_TOPLEFT      0
#define _NET_WM_MOVERESIZE_SIZE_TOP          1
#define _NET_WM_MOVERESIZE_SIZE_TOPRIGHT     2
#define _NET_WM_MOVERESIZE_SIZE_RIGHT        3
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT  4
#define _NET_WM_MOVERESIZE_SIZE_BOTTOM       5
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT   6
#define _NET_WM_MOVERESIZE_SIZE_LEFT         7
#define _NET_WM_MOVERESIZE_MOVE              8   /* movement only */
#define _NET_WM_MOVERESIZE_SIZE_KEYBOARD     9   /* size via keyboard */
#define _NET_WM_MOVERESIZE_MOVE_KEYBOARD    10   /* move via keyboard */
#define _NET_WM_MOVERESIZE_CANCEL           11   /* cancel operation */

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

namespace installer {
namespace {

const char kAtomNameHidden[] = "_NET_WM_STATE_HIDDEN";
const char kAtomNameFullscreen[] = "_NET_WM_STATE_FULLSCREEN";
const char kAtomNameMaximizedHorz[] = "_NET_WM_STATE_MAXIMIZED_HORZ";
const char kAtomNameMaximizedVert[] = "_NET_WM_STATE_MAXIMIZED_VERT";
const char kAtomNameMoveResize[] = "_NET_WM_MOVERESIZE";
const char kAtomNameWmState[] = "_NET_WM_STATE";
const char kAtomNameWmStateAbove[] = "_NET_WM_STATE_ABOVE";
const char kAtomNameWmStateStaysOnTop[] = "_NET_WM_STATE_STAYS_ON_TOP";
const char kAtomNameWmSkipTaskbar[] = "_NET_WM_STATE_SKIP_TASKBAR";
const char kAtomNameWmSkipPager[] = "_NET_WM_STATE_SKIP_PAGER";

const char kWindowTypeName[] = "_NET_WM_WINDOW_TYPE";
const char kWindowTypeDesktop[] = "_NET_WM_WINDOW_TYPE_DESKTOP";
const char kWindowTypeDialog[] = "_NET_WM_WINDOW_TYPE_DIALOG";
const char kWindowTypeDock[] = "_NET_WM_WINDOW_TYPE_DOCK";
const char kWindowTypeMenu[] = "_NET_WM_WINDOW_TYPE_MENU";
const char kWindowTypeNormal[] = "_NET_WM_WINDOW_TYPE_NORMAL";
const char kWindowTypeSplash[] = "_NET_WM_WINDOW_TYPE_SPLASH";
const char kWindowTypeToolbar[] = "_NET_WM_WINDOW_TYPE_TOOLBAR";
const char kWindowTypeUtility[] = "_NET_WM_WINDOW_TYPE_UTILITY";

enum class XCursorType {
  kInvalid = -1,
  kArrow = 2,
  kTop = 138,
  kTopRight = 136,
  kRight = 96,
  kBottomRight = 14,
  kBottom = 16,
  kBottomLeft = 12,
  kLeft = 70,
  kTopLeft = 134,
};

int CornerEdge2WmGravity(const utils::CornerEdge& ce) {
  using namespace utils;
  switch (ce) {
  case CornerEdge::kTopLeft:     return _NET_WM_MOVERESIZE_SIZE_TOPLEFT;
  case CornerEdge::kTop:         return _NET_WM_MOVERESIZE_SIZE_TOP;
  case CornerEdge::kTopRight:    return _NET_WM_MOVERESIZE_SIZE_TOPRIGHT;
  case CornerEdge::kRight:       return _NET_WM_MOVERESIZE_SIZE_RIGHT;
  case CornerEdge::kBottomRight: return _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT;
  case CornerEdge::kBottom:      return _NET_WM_MOVERESIZE_SIZE_BOTTOM;
  case CornerEdge::kBottomLeft:  return _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT;
  case CornerEdge::kLeft:        return _NET_WM_MOVERESIZE_SIZE_LEFT;
  default:                       return _NET_WM_MOVERESIZE_INVALID;
  }
}

XCursorType CornerEdge2XCursor(const utils::CornerEdge& ce) {
  using namespace utils;
  switch (ce) {
  case CornerEdge::kTop:         return XCursorType::kTop;
  case CornerEdge::kTopRight:    return XCursorType::kTopRight;
  case CornerEdge::kRight:       return XCursorType::kRight;
  case CornerEdge::kBottomRight: return XCursorType::kBottomRight;
  case CornerEdge::kBottom:      return XCursorType::kBottom;
  case CornerEdge::kBottomLeft:  return XCursorType::kBottomLeft;
  case CornerEdge::kLeft:        return XCursorType::kLeft;
  case CornerEdge::kTopLeft:     return XCursorType::kTopLeft;
  default:                       return XCursorType::kInvalid;
  }
}

void ChangeWindowMaximizedState(QWidget* widget, int wm_state) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  XEvent xev;
  memset(&xev, 0, sizeof(xev));
  const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
  const Atom vertical_maximized = XInternAtom(display,
                                              kAtomNameMaximizedVert,
                                              false);
  const Atom horizontal_maximized = XInternAtom(display,
                                                kAtomNameMaximizedHorz,
                                                false);

  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = net_wm_state;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();
  xev.xclient.format = 32;

  xev.xclient.data.l[0] = wm_state;
  xev.xclient.data.l[1] = vertical_maximized;
  xev.xclient.data.l[2] = horizontal_maximized;
  xev.xclient.data.l[3] = 1;

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev);
  XFlush(display);
}

void SendMoveResizeMessage(QWidget* widget, int action) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  XEvent xev;
  memset(&xev, 0, sizeof(xev));
  const Atom net_move_resize = XInternAtom(display, kAtomNameMoveResize, false);
  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = net_move_resize;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();
  xev.xclient.format = 32;

  const auto global_position = QCursor::pos();
  xev.xclient.data.l[0] = global_position.x();
  xev.xclient.data.l[1] = global_position.y();
  xev.xclient.data.l[2] = action;
  xev.xclient.data.l[3] = 0;
  xev.xclient.data.l[4] = 0;
  XUngrabPointer(display, QX11Info::appTime());

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev);
  XFlush(display);
}

}  // namespace

namespace utils {

CornerEdge GetCornerEdge(QWidget* widget, int x, int y, int border_width) {
  Q_ASSERT(widget);

  const int window_width = widget->width();
  const int window_height = widget->height();
  unsigned int ce = static_cast<unsigned int>(CornerEdge::kInvalid);

  if (y <= border_width) {
    ce = ce | static_cast<unsigned int>(CornerEdge::kTop);
  }
  if (x <= border_width) {
    ce = ce | static_cast<unsigned int>(CornerEdge::kLeft);
  }
  if (window_height - border_width <= y && y <= window_height) {
    ce = ce | static_cast<unsigned int>(CornerEdge::kBottom);
  }
  if (window_width - border_width <= x && x <= window_width) {
    ce = ce | static_cast<unsigned int>(CornerEdge::kRight);
  }
  return static_cast<CornerEdge>(ce);
}

Qt::MouseButton GetPointerMask() {
  auto display = QX11Info::display();
  auto root_window = QX11Info::appRootWindow();
  Window xwindow;
  Window root, child;
  unsigned int xmask;
  int root_x, root_y, window_x, window_y;

  XSetWindowAttributes attributes;
  xwindow = XCreateWindow(display, root_window, 0, 0, 1,1, 0,
                          CopyFromParent, InputOnly, CopyFromParent,
                          0, &attributes);
  XQueryPointer(display, xwindow, &root, &child, &root_x, &root_y,
                &window_x, &window_y, &xmask);
  XDestroyWindow(display, xwindow);
  switch (xmask) {
    case 256: return Qt::LeftButton;
    case 512: return Qt::MiddleButton;
    case 1024: return Qt::RightButton;
    default: return Qt::NoButton;
  }
}

void GrabFocus(QWidget* widget) {
  XSetInputFocus(QX11Info::display(), widget->winId(),
                 RevertToParent, CurrentTime);
}

void GrabPointer(QWidget* widget) {
  XGrabPointer(QX11Info::display(), widget->winId(), False,
               FocusChangeMask | StructureNotifyMask | PropertyChangeMask,
               GrabModeAsync, GrabModeAsync, widget->winId(), None, CurrentTime);
}

bool IsCornerEdget(QWidget* widget, int x, int y, int border_width) {
  return GetCornerEdge(widget, x, y, border_width) != CornerEdge::kInvalid;
}

void MoveWindow(QWidget* widget) {
  SendMoveResizeMessage(widget, _NET_WM_MOVERESIZE_MOVE);
}

void MoveResizeWindow(QWidget* widget, int x, int y, int border_width) {
  const CornerEdge ce = GetCornerEdge(widget, x, y, border_width);
  if (ce != CornerEdge::kInvalid) {
    const int action = CornerEdge2WmGravity(ce);
    SendMoveResizeMessage(widget, action);
  }
}

void ResizeWindow(QWidget* widget, const QString& corner) {
  if (corner == "right") {
    SendMoveResizeMessage(widget,_NET_WM_MOVERESIZE_SIZE_RIGHT);
  } else if (corner == "bottomright") {
    SendMoveResizeMessage(widget,_NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT);
  }
}

void ResetCursorShape(QWidget* widget) {
 const auto display = QX11Info::display();
 const WId window_id = widget->winId();
 XUndefineCursor(display, window_id);
 XFlush(display);
}

bool SetCursorShape(QWidget* widget, int cursor_id) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const WId window_id = widget->winId();
  const Cursor cursor = XCreateFontCursor(display,
                                          static_cast<unsigned int>(cursor_id));
  if (!cursor) {
    qWarning() << "[ui]::SetCursorShape() call XCreateFontCursor() failed";
    return false;
  }
  const int result = XDefineCursor(display, window_id, cursor);
  XFlush(display);
  return result == Success;
}

void SetMouseTransparent(QWidget* widget, bool on) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  XRectangle XRect;
  XRect.x = 0;
  XRect.y = 0;
  int nRects = 0;

  if (on) {
    XRect.width = 0;
    XRect.height = 0;
    nRects = 0;
  } else {
    XRect.width = widget->width();
    XRect.height = widget->height();
    nRects = 1;
  }
  Q_UNUSED(XRect);
  Q_UNUSED(nRects);
  XShapeCombineRectangles(display, widget->winId(), ShapeInput,
                          0, 0, &XRect, nRects, ShapeSet, YXBanded);
}

void SetStayOnTop(QWidget* widget, bool on) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
  const auto stateAboveAtom = XInternAtom(display, kAtomNameWmStateAbove, false);
  const auto stateStaysOnTopAtom = XInternAtom(display,
                                               kAtomNameWmStateStaysOnTop,
                                               false);

  XEvent xev;
  memset(&xev, 0, sizeof(xev));

  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = wmStateAtom;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();
  xev.xclient.format = 32;

  xev.xclient.data.l[0] = on ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
  xev.xclient.data.l[1] = stateAboveAtom;
  xev.xclient.data.l[2] = stateStaysOnTopAtom;
  xev.xclient.data.l[3] = 1;

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev);
  XFlush(display);
}

void SetWindowTypeHint(QWidget* widget, X11WindowType window_type) {
  // Note that Qt also provides a convinent function, use like this:
  // QXcbWindowFunctions::setWmWindowType(widget->windowHandle(),
  //                                      QXcbWindowFunctions::Normal);
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto win_id = widget->winId();

  Atom atoms[2];
  atoms[0] = XInternAtom(display, kWindowTypeName, False);

  switch (window_type) {
  case X11WindowType::Desktop:
    atoms[1] = XInternAtom(display, kWindowTypeDesktop, False);
    break;

  case X11WindowType::Dialog:
    atoms[1] = XInternAtom(display, kWindowTypeDialog, False);
    break;

  case X11WindowType::Dock:
    atoms[1] = XInternAtom(display, kWindowTypeDock, False);
    break;

  case X11WindowType::Menu:
    atoms[1] = XInternAtom(display, kWindowTypeMenu, False);
    break;

  case X11WindowType::Normal:
    atoms[1] = XInternAtom(display, kWindowTypeNormal, False);
    break;

  case X11WindowType::Splash:
    atoms[1] = XInternAtom(display, kWindowTypeSplash, False);
    break;

  case X11WindowType::Toolbar:
    atoms[1] = XInternAtom(display, kWindowTypeToolbar, False);
    break;

  case X11WindowType::Utility:
    atoms[1] = XInternAtom(display, kWindowTypeUtility, False);
    break;
  }

  XChangeProperty(display, win_id, atoms[0], XA_ATOM, 32,
                  PropModeReplace, (unsigned char *) &atoms[1], 1);
  XMapWindow(display, win_id);
  XFlush(display);
}

void ShowFullscreenWindow(QWidget* widget, bool is_fullscreen) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  XEvent xev;
  memset(&xev, 0, sizeof(xev));
  const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
  const Atom fullscreen = XInternAtom(display, kAtomNameFullscreen, false);
  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = net_wm_state;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();
  xev.xclient.format = 32;

  if (is_fullscreen) {
    xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
  } else {
    xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
  }
  xev.xclient.data.l[1] = fullscreen;
  xev.xclient.data.l[2] = 0;
  xev.xclient.data.l[3] = 1;

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev
  );
  XFlush(display);
}

void ShowMaximizedWindow(QWidget* widget) {
  ChangeWindowMaximizedState(widget, _NET_WM_STATE_ADD);
}

void ShowMinimizedWindow(QWidget* widget, bool minimized) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  XEvent xev;
  memset(&xev, 0, sizeof(xev));
  const Atom net_wm_state = XInternAtom(display, kAtomNameWmState, false);
  const Atom hidden = XInternAtom(display, kAtomNameHidden, false);
  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = net_wm_state;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();
  xev.xclient.format = 32;

  if (minimized) {
    xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
  } else {
    xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
  }
  xev.xclient.data.l[1] = hidden;
  xev.xclient.data.l[2] = 0;
  xev.xclient.data.l[3] = 1;

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev
  );
  XIconifyWindow(display, widget->winId(), screen);
  XFlush(display);
}

void ShowNormalWindow(QWidget* widget) {
  ChangeWindowMaximizedState(widget, _NET_WM_STATE_REMOVE);
}

void ToggleMaximizedWindow(QWidget* widget) {
  ChangeWindowMaximizedState(widget, _NET_WM_STATE_TOGGLE);
}

bool UpdateCursorShape(QWidget* widget, int x, int y, int border_width) {
  const CornerEdge ce = GetCornerEdge(widget, x, y, border_width);
  const XCursorType x_cursor = CornerEdge2XCursor(ce);
  if (x_cursor != XCursorType::kInvalid) {
    return SetCursorShape(widget, static_cast<unsigned int>(x_cursor));
  } else {
    return false;
  }
}

void SkipTaskbarPager(QWidget* widget) {
  Q_ASSERT(widget);

  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  const auto wmStateAtom = XInternAtom(display, kAtomNameWmState, false);
  const auto taskBarAtom = XInternAtom(display, kAtomNameWmSkipTaskbar, false);
  const auto noPagerAtom = XInternAtom(display, kAtomNameWmSkipPager, false);

  XEvent xev;
  memset(&xev, 0, sizeof(xev));

  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = wmStateAtom;
  xev.xclient.display = display;
  xev.xclient.window = widget->winId();;
  xev.xclient.format = 32;

  xev.xclient.data.l[0] = _NET_WM_STATE_ADD;
  xev.xclient.data.l[1] = taskBarAtom;
  xev.xclient.data.l[2] = noPagerAtom;
  xev.xclient.data.l[3] = 1;

  XSendEvent(display,
             QX11Info::appRootWindow(screen),
             false,
             SubstructureRedirectMask | SubstructureNotifyMask,
             &xev);
  XFlush(display);
}

void  SimulateButtonEvent(Qt::MouseButton button, bool is_press) {
  const auto display = QX11Info::display();
  const unsigned int x11_button = static_cast<unsigned int>(button);
  XTestFakeButtonEvent(display, x11_button, is_press, CurrentTime);
}

void SimulateMotionEvent(int x, int y) {
  const auto display = QX11Info::display();
  const auto screen = QX11Info::appScreen();

  XTestFakeMotionEvent(display, screen, x, y, CurrentTime);
}

}  // namespace utils
}  // namespace installer
