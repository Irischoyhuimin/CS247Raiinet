#include "xwindow.h"
#include <iostream>
#include <unistd.h>  // for usleep
using namespace std;
Xwindow::Xwindow(int width, int height) {
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        throw runtime_error("Cannot open X display");
    }

    int scr = DefaultScreen(dpy);
    unsigned long white = WhitePixel(dpy, scr);
    unsigned long black = BlackPixel(dpy, scr);

    win = XCreateSimpleWindow(
        dpy, DefaultRootWindow(dpy),
        10, 10,                          // x, y
        width, height,                   // width, height
        1,                               // border width
        black, white
    );

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapRaised(dpy, win);

    // Create a GC
    Pixmap pix = XCreatePixmap(dpy, win, width, height, DefaultDepth(dpy, scr));
    gc = XCreateGC(dpy, pix, 0, (XGCValues*)0);

    // Initialize colours array
    XColor xcolour;
    Colormap cmap = DefaultColormap(dpy, scr);
    const char* names[11] = {
        "white", "black", "red", "green", "blue",
        "cyan", "yellow", "magenta", "orange", "brown", "pink"
    };
    for (int i = 0; i < 11; ++i) {
        if (!XParseColor(dpy, cmap, names[i], &xcolour) ||
            !XAllocColor(dpy, cmap, &xcolour)) {
            // Fallback: use white/black if allocation fails
            colours[i] = (i == Black) ? black : white;
        } else {
            colours[i] = xcolour.pixel;
        }
    }

    XSetForeground(dpy, gc, colours[Black]);
    XFlush(dpy);
    usleep(1000); // small delay so window appears
}

Xwindow::~Xwindow() {
    if (gc)  XFreeGC(dpy, gc);
    if (win) XDestroyWindow(dpy, win);
    if (dpy) XCloseDisplay(dpy);
}

void Xwindow::fillRectangle(int x, int y, int w, int h, int colour) {
    XSetForeground(dpy, gc, colours[colour]);
    XFillRectangle(dpy, win, gc, x, y, w, h);
    XSetForeground(dpy, gc, colours[Black]); // restore default
    XFlush(dpy);
}

void Xwindow::drawString(int x, int y, const string& msg, int colour) {
    XSetForeground(dpy, gc, colours[colour]);
    XDrawString(dpy, win, gc, x, y, msg.c_str(), static_cast<int>(msg.size()));
    XSetForeground(dpy, gc, colours[Black]); // restore default
    XFlush(dpy);
}

void Xwindow::drawLine(int x1, int y1, int x2, int y2, int colour) {
    XSetForeground(dpy, gc, colours[colour]);
    XDrawLine(dpy, win, gc, x1, y1, x2, y2);
    XSetForeground(dpy, gc, colours[Black]); // restore default
    XFlush(dpy);
}

unsigned long Xwindow::getColour(int c) const {
    return colours[c];
}