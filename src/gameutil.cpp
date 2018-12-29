//
// Created by Martin Eberle on 25.12.2018.
//

#include "gameutil.h"

GameException::GameException(const std::string &message) : message{message} {}

const char *GameException::what() const noexcept {
    return message.empty() ? "Game error" : message.c_str();
}
