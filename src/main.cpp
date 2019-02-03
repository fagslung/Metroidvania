#include <iostream>
#include "game.h"
#undef main
#include <SDL.h>


int main(int argc, const char **argv) {
    try {
        Metroidvania metroidvania;
        std::cout << "hello";

        return 0;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;

        return -1;
    }
}