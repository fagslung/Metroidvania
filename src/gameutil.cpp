//
// Created by Martin Eberle on 25.12.2018.
//

#include <SDL.h>
#include "gameutil.h"

GameException::GameException(const std::string &message) : message{message} {}

const char *GameException::what() const noexcept {
    return message.empty() ? "Game error" : message.c_str();
}

void logSDLError(std::ostream &os, const std::string &msg) {
    //os << msg << " error: " << SDL_GetError() << std::endl;
}

const char *GameInitException::what() const noexcept {
    if (message.empty()) {
        return "Game failed to initialize";
    } else {
        return GameException::what();
    }
}

GameInitException::GameInitException(const std::string &message) : GameException(message) {

}
