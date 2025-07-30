#include "controller.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    bool graphics = false;
    bool textOnly = true;
    std::string link1File, link2File;
    std::string abilityOrder1, abilityOrder2;

    // Simple flag parsing
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-graphics") {
            graphics = true;
            textOnly = false;
        } else if (arg == "-link1" && i+1 < argc) {
            link1File = argv[++i];
        } else if (arg == "-link2" && i+1 < argc) {
            link2File = argv[++i];
        } else if (arg == "-ability1" && i+1 < argc) {
            abilityOrder1 = argv[++i];
        } else if (arg == "-ability2" && i+1 < argc) {
            abilityOrder2 = argv[++i];
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    Xwindow* xw = nullptr;
    Controller* controller = nullptr;

    if (graphics) {
        // Create an actual X window; pick any reasonable size
        xw = new Xwindow(400, 400+120);
        controller = new Controller(xw);     // <-- uses Game(Xwindow*)
    } else {
        controller = new Controller(true);   // <-- text-only path
    }

    // Apply initial setup flags BEFORE start
    if (!link1File.empty()) controller->processCommand("setup link1 " + link1File);
    if (!link2File.empty()) controller->processCommand("setup link2 " + link2File);
    if (!abilityOrder1.empty()) controller->processCommand("setup abilities1 " + abilityOrder1);
    if (!abilityOrder2.empty()) controller->processCommand("setup abilities2 " + abilityOrder2);

    controller->processCommand("start");  // create board & begin game
    controller->run();

    delete controller;
    delete xw;
    return 0;

}
