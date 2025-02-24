#include <X11/X.h>
#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define LENGTH(X) (sizeof X / sizeof X[0])

typedef struct {
  const char *class;
  char *inputMask;
  char *inputKey;
  char *outputMask;
  char *outputKey;
} Remap;

#include "config.h"

char *getwindowclass(Display *display, Window w, XClassHint ch) {
  XGetClassHint(display, w, &ch);
  return ch.res_class;
}

void fakepresskey(Display *display, Window win, char *typemask, char *strkey) {
  XEvent ev;

  memset(&ev, 0x00, sizeof(ev));
  ev.xkey.display = display;
  ev.xkey.window = win;
  ev.xkey.subwindow = None;

  if (strcmp(typemask, "ControlMask") == 0) {
    ev.xkey.state = ControlMask;
  } else if (strcmp(typemask, "ShiftMask") == 0) {
    ev.xkey.state = ShiftMask;
  } else if (strcmp(typemask, "Mod1Mask") == 0) {
    ev.xkey.state = Mod1Mask;
  } else if (strcmp(typemask, "Mod4Mask") == 0) {
    ev.xkey.state = Mod4Mask;
  }

  ev.xkey.keycode = XKeysymToKeycode(display, XStringToKeysym(strkey));
  ev.xkey.same_screen = True;

  ev.xkey.type = KeyPress;
  XSendEvent(display, win, True, KeyPressMask, &ev);
  XFlush(display);

  ev.xkey.type = KeyRelease;
  XSendEvent(display, win, True, KeyReleaseMask, &ev);
  XFlush(display);
}

int main(int argc, char **argv) {
  Display *display;
  Window focus;
  int revert;
  XClassHint ch;
  unsigned int i;
  const Remap *r;

  if (argc != 3)
    return 1;

  display = XOpenDisplay(NULL);
  XGetInputFocus(display, &focus, &revert);
  XGetClassHint(display, focus, &ch);

  for (i = 0; i < LENGTH(remaps); i++) {
    r = &remaps[i];
    if (strcmp(getwindowclass(display, focus, ch), r->class) == 0) {
      if ((strcmp(r->inputMask, argv[1]) == 0) &&
          (strcmp(r->inputKey, argv[2]) == 0)) {
        fakepresskey(display, focus, r->outputMask, r->outputKey);
        XCloseDisplay(display);
        return 0;
      }
    }
  }
  fakepresskey(display, focus, argv[1], argv[2]);
  XCloseDisplay(display);
  return 1;
}
