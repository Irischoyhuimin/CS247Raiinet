// #include "controller.h"
// int main(int argc, char* argv[]) {
//     bool graphics = false;
//     // Parse args here to set flags

//     Controller controller(graphics);
//     controller.run();
//     return 0;
// }
#include "board.h"
#include "player.h"
#include "link.h"

int main() {
    Player p1("Player1");
    Player p2("Player2");
    Board board;

    board.setup();

    Link* l1 = new Virus('a', &p1, 2);
    Link* l2 = new Data('A', &p2, 1);

    p1.addLink(l1);
    p2.addLink(l2);

    board.getGrid()->at(1)[0].setLink(l1);
    board.getGrid()->at(2)[0].setLink(l2);

    board.move(&p1, &p2, *l1, 2, 0);  

    board.printTextDisplay();
    return 0;
}