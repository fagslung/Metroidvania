//
// Created by Martin Eberle on 30.12.2018.
//

#include <iostream>
#include "game.h"
#include "gameutil.h"

Metroidvania::Metroidvania() {
    initSdl();

    // TODO ...
}

Metroidvania::~Metroidvania() {
    destructSdl();
}

void Metroidvania::destructSdl() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    SDL_Quit();
}

void Metroidvania::initSdl() {
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");

        throw GameInitException();
    }

    // open window
    window = SDL_CreateWindow("Metroidvania", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "SDL_CreateWindow");

        throw GameInitException();
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "SDL_CreateRenderer");

        throw GameInitException();
    }

    // initialize PNG loading
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError(std::cout, "IMG_Init");

        throw GameInitException();
    }

    std::cout << "SDL successfully initialized" << std::endl;
}
