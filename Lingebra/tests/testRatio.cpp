#include "../ratio.h"


int main(int argc, char* argv[])
{
    //declare rational 0;
    rational<int> zero;
    rational<int> one{1};

    print("{}\n", zero);
    print("{}\n", one);
}