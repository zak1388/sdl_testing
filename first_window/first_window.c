/*
 * =====================================================================================
 *
 *       Filename:  first_window.c
 *
 *    Description: first time making a window in SDL lesss gooo 
 *
 *        Version:  1.0
 *        Created:  21/01/22 15:46:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h" 

int main(void)
{
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(
    "First SDL window",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    640,
    480,
    SDL_WINDOW_RESIZABLE
  );
  
  if (window == NULL)
  {
    fprintf(stderr, "Couldn't make window: %s\n", SDL_GetError());
    exit(1);
  }

  renderer = SDL_CreateRenderer(
      window,
      -1,
      0
  );

  if (renderer == NULL)
  {
    fprintf(stderr, "Couldn't initialise renderer: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);

  int quit = false; 
  do{
    SDL_Event event;
    if (SDL_PollEvent(&event)) 
    {
        switch(event.type) 
        {
          case SDL_QUIT: {
            quit = true;
            break;
           }
        }
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  } while(!quit);

  SDL_Quit();
  return 0;
}

