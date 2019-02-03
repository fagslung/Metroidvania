//
// Created by Martin Eberle on 25.12.2018.
//

#ifndef METROIDVANIA_GAMEUTIL_H
#define METROIDVANIA_GAMEUTIL_H

#include <stdio.h>
#include <string>


/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream& os, const std::string& msg);


/**
 *
 */
struct GameException : public std::exception {
public:
    explicit GameException() = default;
    explicit GameException(const std::string& message);

    const char* what() const noexcept override;

protected:
    std::string message;
};

/**
 * Exception thrown when Game initialisation went wrong
 * e.g. SDL library initialisation, loading of images
 */
struct GameInitException : public GameException {
public:
    explicit GameInitException() = default;
    explicit GameInitException(const std::string& message);

    const char* what() const noexcept override;
};




#endif //METROIDVANIA_GAMEUTIL_H
