#include "controller.h"
#include <iostream>
#include <sstream>
#include <fstream>

// Utility: split comma-separated list
static vector<string> splitList(const string& s) {
    vector<string> out;
    string token;
    istringstream iss(s);
    while (getline(iss, token, ',')) {
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
    cout << "Welcome to RAIInet! Type 'help' for commands." << endl;
    string line;
    while (true) {
        if(gameOver) break;
        cout << (setupMode ? "(setup)> " : "> ");
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        if (line == "quit") break;
        processCommand(line);
    }
}

void Controller::processCommand(const string& cmd) {
    istringstream iss(cmd);
    vector<string> tokens;
    for (string w; iss >> w; ) tokens.push_back(w);
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
            cout << "Game already started." << endl;
        }
    } else if (setupMode) {
        cout << "Unknown setup command '" << c << "'. Type 'help'." << endl;
    } else if (c == "ability") {
        parseAbility(tokens);
    } else if (c == "abilities") {
        printAbilities();
    } else if (c == "move") {
        parseMove(tokens);
    } else if (c == "board") {
        game.display();
    } else if (c == "sequence") {
        parseSequence(tokens);
    } else if (c == "help") {
        printHelp();
    } else {
        cout << "Unknown command '" << c << "'. Type 'help'." << endl;
    }
}

void Controller::parseMove(const vector<string>& args) {
    if (args.size() != 3) {
        cout << "Usage: move <direction> <linkId> (e.g., move up a)" << endl;
        return;
    }
    abilityUsedThisTurn = false;  // Reset after ability use
    game.move(args[1], args[2]);
    if (game.isGameOver()) {
        int w = game.getWinner();
        if (w == 1) cout << "Game over! Player 1 wins!\n";
        else if (w == 2) cout << "Game over! Player 2 wins!\n";
        gameOver = true; // so run() breaks next prompt
        return;
    }
    game.display();
}

void Controller::parseAbility(const vector<string>& args) {
    if (abilityUsedThisTurn) {
        cout << "Ability already used this turn." << endl;
        return;
    }
    if (args.size() < 2) {
        cout << "Usage: ability <index> [args] (index from 1)" << endl;
        return;
    }
    vector<string> abilityArgs(args.begin() + 2, args.end());
    int before = game.getCurrentPlayer().getAbilityUsesLeft();
    game.useAbility(args[1], abilityArgs);
    int after = game.getCurrentPlayer().getAbilityUsesLeft();
    if (game.isGameOver()) {
        int w = game.getWinner();
        if (w == 1) cout << "Game over! Player 1 wins!\n";
        else if (w == 2) cout << "Game over! Player 2 wins!\n";
        gameOver = true;
        return;
    }
    if(after < before){
        abilityUsedThisTurn = true;
    }
    game.display();
}

void Controller::parseSequence(const vector<string>& args) {
    if (args.size() != 2) {
        cout << "Usage: sequence <file>" << endl;
        return;
    }
    ifstream infile(args[1]);
    if (!infile) {
        cout << "Could not open sequence file: " << args[1] << endl;
        return;
    }
    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;
        processCommand(line);
        if (game.isGameOver()) break;
    }
}

void Controller::startGameIfReady() {
    if (!link1Loaded) {
        game.randomizeLinksForPlayer(1);
        cout << "Player 1 links randomized.\n";
    }
    if (!link2Loaded) {
        game.randomizeLinksForPlayer(2);
        cout << "Player 2 links randomized.\n";
    }
    game.init();
    game.start();
    setupMode = false;
    abilityUsedThisTurn = false;
}

void Controller::parseSetup(const vector<string>& args) {
    if (args.size() < 2) {
        cout << "Usage: setup <option> [params]" << endl;
        return;
    }
    const auto& opt = args[1];
    if (opt == "link1" && args.size() == 3) {
        game.loadLinkConfig(1, args[2]);
        link1Loaded = true;  // Mark link1 loaded
        cout << "Link1 configuration loaded." << endl;
    } else if (opt == "link2" && args.size() == 3) {
        game.loadLinkConfig(2, args[2]);
        link2Loaded = true;  // Mark link2 loaded
        cout << "Link2 configuration loaded." << endl;
    } else if (opt == "abilities1" && args.size() == 3) {
        auto order = splitList(args[2]);
        game.setAbilityOrder(1, order);
        cout << "Abilities set for player1." << endl;
    } else if (opt == "abilities2" && args.size() == 3) {
        auto order = splitList(args[2]);
        game.setAbilityOrder(2, order);
        cout << "Abilities set for player2." << endl;
    } else if (opt == "enhancements") {
        game.toggleEnhancements();
    } else {
        cout << "Unknown setup option '" << opt << "'. Type 'help'." << endl;
    }
}

void Controller::printAbilities() {
    auto& abilities = game.getCurrentPlayer().getAbilities(); 
    cout << "Abilities:\n";
    for (int i = 0; i < (int)abilities.size(); ++i) {
        const auto& ability = abilities[i];
        cout << (i+1) << ": " << ability->getType()
                  << " (" << (ability->isUsed() ? "used" : "unused") << ")\n";
    }
}


void Controller::printHelp() {
    cout << "Commands:\n"
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
