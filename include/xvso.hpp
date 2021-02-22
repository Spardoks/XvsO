#ifndef XVSO_HPP
#define XVSO_HPP

#include <vector>

class XvsO {

  public:

    static unsigned gFieldSize_;
    static unsigned gWinChainLength_;

    static void showMenu();

    static void inputConfig();

    XvsO();

    void start();

  private:

    void printField();

    bool inputMove(unsigned & x, unsigned & y);

    void doMove(unsigned x, unsigned y);

    void checkStatus();

    void finish();

    const unsigned cFieldSize_;
    const unsigned cWinChainLength_;

    std::vector<std::vector<char>> field_;
    unsigned moveCount_;
    bool playerOne_;
    bool end_;
};

#endif // XVSO_HPP
