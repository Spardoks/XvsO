#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "xvso.hpp"

void XvsO::showMenu()
{
    // system("clear"); // system("cls");
    std::cout << "-------------------------------------------------------\n";
    std::cout << "Game XvsO:\n";
    std::cout << "Mode: " << gFieldSize_ << "x" << gFieldSize_ << ", ";
    std::cout << gWinChainLength_ << " in row to win\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << "Print 0 to start or print 1 to configure game\n";
}



void XvsO::inputConfig() {

    unsigned int mode = 0;
    bool goodInput = false;
    while (!goodInput) {
        std::cin >> mode;
        if ( (std::cin.fail()) ||
             (std::cin.peek() != '\n') ||
             !((mode == 0) || (mode == 1)) ) {
            std::cin.clear();
        } else {
            goodInput = true;
        }
        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (mode == 0) {
        // Standart params
        return;
    }

    goodInput = false;
    while (!goodInput) {
        std::cout << "Input size of field ";
        std::cout << "(3, 4, ... 10): ";
        std::cin >> XvsO::gFieldSize_;
        if ( (std::cin.fail()) ||
             (std::cin.peek() != '\n') ||
             !((XvsO::gFieldSize_ > 2) &&
                (XvsO::gFieldSize_ <= 10)) ) {
            std::cin.clear();
        } else {
            goodInput = true;
        }
        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    goodInput = false;
    while (!goodInput) {
        std::cout << "Input count in row to win ";
        std::cout << "(3 or more, but less or equal than size of field): ";
        std::cin >> XvsO::gWinChainLength_;
        if ( (std::cin.fail()) ||
             (std::cin.peek() != '\n') ||
             !((XvsO::gWinChainLength_> 2) &&
                (XvsO::gWinChainLength_ <= XvsO::gFieldSize_)) ) {
            std::cin.clear();
        } else {
            goodInput = true;
        }
        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

}

XvsO::XvsO()
    : cFieldSize_(XvsO::gFieldSize_),
      cWinChainLength_(XvsO::gWinChainLength_),
      field_(cFieldSize_, std::vector<char>(cFieldSize_, 0)),
      moveCount_(0),
      playerOne_(true),
      end_(false)
{
}



void XvsO::start()
{
    printField();

    while (!end_) {
        unsigned x = 0;
        unsigned y = 0;
        bool canMove = (inputMove(x, y)) && (field_[y - 1][x - 1] == 0);
        if (!canMove) {
            std::cout << "Bad moving! Again:\n";
            continue;
        }

        doMove(x, y);
        printField();
        checkStatus();
    }

    finish();
}



// Private

void XvsO::printField()
{
    //system("clear"); // system("cls");
    if (playerOne_) {
        std::cout << "\nPlayer: X\n\n";
    } else {
        std::cout << "\nPlayer: O\n\n";
    }
    std::cout << "   ";
    for (unsigned i = 0; i < cFieldSize_; ++i) {
        std::cout << " x ";
    }
    std::cout << "\n";
    std::cout << "   ";
    for (unsigned i = 0; i < cFieldSize_; ++i) {
        std::cout << " " << i + 1 << " ";
    }
    std::cout << "\n\n";
    for (unsigned i = 0; i < cFieldSize_; ++i) {
        std::cout << "y" << i + 1 << " ";
        for (unsigned j = 0; j < cFieldSize_; ++j) {
            switch (field_[i][j]) {
              case 0:
                std::cout << " * ";
                break;
              case -1:
                std::cout << " O ";
                break;
              case 1:
                std::cout << " X ";
                break;
              default:
                // Smth bad
                std::cout << "Smth bad\n";
                exit(-1);
                break;
            }
        }
        std::cout << "\n\n";
    }
}



bool XvsO::inputMove(unsigned& x, unsigned& y)
{
    std::cout << "MoveTo(x , y): ";

    std::string line;
    if (std::cin.peek() == '\n') {
        std::cin.ignore(1);
    }
    std::getline(std::cin, line);
    std::istringstream sstream(line);
    unsigned int rows = 0;
    unsigned int collumns = 0;
    if (sstream >> rows &&
        rows <= cFieldSize_ &&
        sstream >> collumns &&
        collumns <= cFieldSize_ &&
        sstream.eof()) {

        x = rows;
        y = collumns;

        return true;
    }

    return false;
}



void XvsO::doMove(unsigned x, unsigned y)
{
    if (playerOne_) {
        field_[y - 1][x - 1] = 1;
    } else {
        field_[y - 1][x - 1] = -1;
    }
    playerOne_ = !playerOne_;
    ++moveCount_;
}



void XvsO::checkStatus()
{
    if (moveCount_ == (cFieldSize_ * cFieldSize_)) {
        end_ = true;
        return;
    }

    int lastMove = 1;
    if (playerOne_ == true) {
        lastMove = -1;
    }

    unsigned chainLength = 1;
    for (unsigned i = 0; i < cFieldSize_; ++i) {
        for (unsigned j = 0; j < cFieldSize_; ++j) {
            if (field_[i][j] != lastMove) {
                continue;
            }

            // Right
            if ( (cFieldSize_ - j) >= cWinChainLength_ ) {
                for (unsigned k = 1; k < cWinChainLength_; ++k) {
                    if (field_[i][j + k] != lastMove) {
                        break;
                    }
                    ++chainLength;
                }
                if (chainLength == cWinChainLength_) {
                    end_ = true;
                    return;
                }
                chainLength = 1;
            }

            // Down
            if ( (cFieldSize_ - i) >= cWinChainLength_ ) {
                for (unsigned k =  1; k < cWinChainLength_; ++k) {
                    if (field_[i + k][j] != lastMove) {
                        break;
                    }
                    ++chainLength;
                }
                if (chainLength == cWinChainLength_) {
                    end_ = true;
                    return;
                }
                chainLength = 1;
            }

            // Diag-Right
            if ( ((cFieldSize_ - j) >= cWinChainLength_) &&
                ((cFieldSize_ - i) >= cWinChainLength_) ) {
                for (unsigned k = 1; k < cWinChainLength_; ++k) {
                    if (field_[i + k][j + k] != lastMove) {
                        break;
                    }
                    ++chainLength;
                }
                if (chainLength == cWinChainLength_) {
                    end_ = true;
                    return;
                }
                chainLength = 1;
            }

            // Diag-Left
            if ( (j + 1 >= cWinChainLength_) &&
                ((cFieldSize_ - i) >= cWinChainLength_) ) {
                for (unsigned k = 1; k < cWinChainLength_; ++k) {
                    if (field_[i + k][j - k] != lastMove) {
                        break;
                    }
                    ++chainLength;
                }
                if (chainLength == cWinChainLength_) {
                    end_ = true;
                    return;
                }
                chainLength = 1;
            }
        }
    }
}



void XvsO::finish()
{
    std::cout << "\n\nGAME OVER! ";
    if (moveCount_ == (cFieldSize_ * cFieldSize_)) {
        std::cout << "DRAW";
    } else {
        std::cout << "Player ";
        if (playerOne_) {
            std::cout << "O";
        } else {
            std::cout << "X";
        }
        std::cout << " is winner!";

    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
