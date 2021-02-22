#include <iostream>

#include "xvso.hpp"

unsigned XvsO::gFieldSize_ = 3;
unsigned XvsO::gWinChainLength_ = 3;

int main() {

    XvsO::showMenu();
    XvsO::inputConfig();
    XvsO game;
    game.start();

    return 0;
}
