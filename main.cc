#include "controller.h"
int main(int argc, char* argv[]) {
    bool graphics = false;
    // Parse args here to set flags

    Controller controller(graphics);
    controller.run();
    return 0;
}