#ifndef XWINDOW_H
#define XWINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;
class Xwindow {
    Display*     dpy;
    Window       win;
    GC           gc;
    unsigned long colours[11];

  public:
    enum {
      White = 0, Black, Red, Green, Blue,
      Cyan, Yellow, Magenta, Orange, Brown, Pink
    };

    Xwindow(int width = 500, int height = 500);

    ~Xwindow();

    void fillRectangle(int x, int y, int w, int h, int colour);

    void drawString(int x, int y, const string& msg, int colour = Black);

    void drawLine(int x1, int y1, int x2, int y2, int colour = Black);

    unsigned long getColour(int c) const;
};
#endif
