#include "observer.h"

class Board;
class Xwindow;

class TextDisplay : public Observer {
    Board* board;

public:
    TextDisplay(Board* b);
    void update() override;
    void print();
};

class GraphicsDisplay : public Observer {
    Board* board;
    Xwindow* window;

public:
    GraphicsDisplay(Board* b, Xwindow* xw);
    void update() override;
    void redrawCell(int x, int y);
    void redrawBoard();
};
