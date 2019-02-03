//
// Created by Martin Eberle on 30.12.2018.
//

#ifndef METROIDVANIA_GAME_H
#define METROIDVANIA_GAME_H


#include <SDL.h>
#include <SDL_image.h>


/**
 *
 */
 class Metroidvania {
 public:
     Metroidvania();
     ~Metroidvania();

 private:
     void initSdl();
     void destructSdl();

     const int SCREEN_WIDTH  = 1000;
     const int SCREEN_HEIGHT = 500;

     SDL_Window* window;
     SDL_Renderer* renderer;

 };

#endif //METROIDVANIA_GAME_H
