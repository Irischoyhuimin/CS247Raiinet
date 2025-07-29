#include "controller.h"
#include <iostream>
#include <sstream>
#include <fstream>

// Utility: split comma-separated list
static std::vector<std::string> splitList(const std::string& s) {
    std::vector<std::string> out;
    std::string token;
    std::istringstream iss(s);
    while (std::getline(iss, token, ',')) {
        if (!token.empty()) out.push_back(token);
    }
    return out;
}

Controller::Controller(bool textOnly)
    : game(textOnly), setupMode(true), textOnly(textOnly),
      xw(nullptr), abilityUsedThisTurn(false) {}

Controller::Controller(Xwindow* xw)
    : game(xw), setupMode(true), textOnly(false),
      xw(xw), abilityUsedThisTurn(false) {}

void Controller::run() {
    std::cout << "Welcome to RAIInet! Type 'help' for commands." << std::endl;
    std::string line;
    while (true) {
        std::cout << (setupMode ? "(setup)> " : "> ");
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "quit") break;
        processCommand(line);
        if (!setupMode && game.isGameOver()) {
            std::cout << "Game over!" << std::endl;
            break;
        }
    }
}

void Controller::processCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> tokens;
    for (std::string w; iss >> w; ) tokens.push_back(w);
    if (tokens.empty()) return;
    const auto& c = tokens[0];

    if (c == "setup") {
        parseSetup(tokens);
    } else if (c == "start") {
        if (setupMode) {
            game.init();
            game.start();
            setupMode = false;
            abilityUsedThisTurn = false;
        } else {
            std::cout << "Game already started." << std::endl;
        }
    } else if (setupMode) {
        std::cout << "Unknown setup command '" << c << "'. Type 'help'." << std::endl;
    } else if (c == "ability") {
        parseAbility(tokens);
    } else if (c == "abilities") {
        printAbilities();
    }
    else if (c == "move") {
        parseMove(tokens);
    } else if (c == "board") {
        game.display();
    } else if (c == "sequence") {
        parseSequence(tokens);
    } else if (c == "help") {
        printHelp();
    } else {
        std::cout << "Unknown command '" << c << "'. Type 'help'." << std::endl;
    }
}

void Controller::parseMove(const std::vector<std::string>& args) {
    if (args.size() != 3) {
        std::cout << "Usage: move <direction> <linkId> (e.g., move up a)" << std::endl;
        return;
    }
    abilityUsedThisTurn = false;  // Reset after ability use
    game.move(args[1], args[2]);
    game.display();
}

void Controller::parseAbility(const std::vector<std::string>& args) {
    if (abilityUsedThisTurn) {
        std::cout << "Ability already used this turn." << std::endl;
        return;
    }
    if (args.size() < 2) {
        std::cout << "Usage: ability <index> [args] (index from 1)" << std::endl;
        return;
    }
    std::vector<std::string> abilityArgs(args.begin() + 2, args.end());
    game.useAbility(args[1], abilityArgs);
    abilityUsedThisTurn = true;
    game.display();
}

void Controller::parseSequence(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Usage: sequence <file>" << std::endl;
        return;
    }
    std::ifstream infile(args[1]);
    if (!infile) {
        std::cout << "Could not open sequence file: " << args[1] << std::endl;
        return;
    }
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        processCommand(line);
        if (game.isGameOver()) break;
    }
}

void Controller::parseSetup(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: setup <option> [params]" << std::endl;
        return;
    }
    const auto& opt = args[1];
    if (opt == "link1" && args.size() == 3) {
        game.loadLinkConfig(1, args[2]);
        std::cout << "Link1 configuration loaded." << std::endl;
    } else if (opt == "link2" && args.size() == 3) {
        game.loadLinkConfig(2, args[2]);
        std::cout << "Link2 configuration loaded." << std::endl;
    } else if (opt == "abilities1" && args.size() == 3) {
        auto order = splitList(args[2]);
        game.setAbilityOrder(1, order);
        std::cout << "Abilities set for player1." << std::endl;
    } else if (opt == "abilities2" && args.size() == 3) {
        auto order = splitList(args[2]);
        game.setAbilityOrder(2, order);
        std::cout << "Abilities set for player2." << std::endl;
    } else if (opt == "enhancements") {
        game.toggleEnhancements();
    } else {
        std::cout << "Unknown setup option '" << opt << "'. Type 'help'." << std::endl;
    }
}

void Controller::printAbilities() {
    auto& abilities = game.getCurrentPlayer().getAbilities(); 
    std::cout << "Abilities:\n";
    for (int i = 0; i < (int)abilities.size(); ++i) {
        const auto& ability = abilities[i];
        std::cout << (i+1) << ": " << ability->getType()
                  << " (" << (ability->isUsed() ? "used" : "unused") << ")\n";
    }
}


void Controller::printHelp() {
    std::cout << "Commands:\n"
              << "  setup link1 <file>           Load link1 configuration\n"
              << "  setup link2 <file>           Load link2 configuration\n"
              << "  setup abilities1 <list>      Comma-separated ability list for P1\n"
              << "  setup abilities2 <list>      Comma-separated ability list for P2\n"
              << "  setup enhancements           Toggle enhancements\n"
              << "  start                        Begin gameplay\n"
              << "  abilities                    Show your ability cards and their usage status\n"
              << "  ability <index> [args]       Use ability by index (1-based)\n"
              << "  move <direction> <linkId>    Move link after any ability\n"
              << "  board                        Display board\n"
              << "  sequence <file>              Run commands from file\n"
              << "  help                         Show help\n"
              << "  quit                         Exit\n";
}
