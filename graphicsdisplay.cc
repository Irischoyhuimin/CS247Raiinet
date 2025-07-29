// graphicsdisplay.cc
#include "display.h"

// implement GraphicsDisplay methods here
GraphicsDisplay::GraphicsDisplay(Board* b, Xwindow* xw) : board(b), window(xw) {}
void GraphicsDisplay::update() { /* ... */ }
void GraphicsDisplay::redrawCell(int x, int y) { /* ... */ }
void GraphicsDisplay::redrawBoard() { /* ... */ }
