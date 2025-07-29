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

    Controller controller{textOnly};
    // Apply initial setup flags
    if (!link1File.empty()) controller.processCommand("setup link1 " + link1File);
    if (!link2File.empty()) controller.processCommand("setup link2 " + link2File);
    if (!abilityOrder1.empty()) controller.processCommand("setup abilities1 " + abilityOrder1);
    if (!abilityOrder2.empty()) controller.processCommand("setup abilities2 " + abilityOrder2);

    controller.processCommand("start");  // auto-start after setup
    controller.run();
    return 0;
}
