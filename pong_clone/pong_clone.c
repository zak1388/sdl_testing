/*
 * =====================================================================================
 *
 *       Filename:  pong_clone.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  21/01/22 19:12:58
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

/*
 * Position of paddles and ball is normalised
 * so i dont have to pass around the window and deal with resizing and stuff
 *
 */

#define BALL_RADIUS 0.05f
typedef struct
{
  float x;
  float y;
  
  // dx and dy are ball velocities
  float dx;
  float dy;
} Ball;

#define PADDLE_SPEED 0.66f
#define PADDLE_LENGTH 0.2f
#define PADDLE_WIDTH 0.01f
typedef struct
{
  float x, y;
  float w, h;
} NormRect;

typedef struct 
{
  Ball* ball;
  NormRect *player1, *player2;
  size_t[2] score;
  bool paused;
} GameState;

void next_frame(GameState* gamestate, int player1_move_dir, int player2_move_dir);
GameState* InitGameState();
int CalcPlayer2Move(GameState* gamestate);
float rand_speed();
void reset_ball(GameState* gamestate)

#define PLAYER1_MOVEUP SDL_SCANCODE_W
#define PLAYER1_MOVEDOWN SDL_SCANCODE_S

int main(void)
{
  SDL_Window* window;
  GameState* gamestate;
  SDL_Renderer* renderer;

  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow(
      "Pong clone :P",
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

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  gamestate = InitGameState();

  bool quit = false;
  int player1_movedir = 0;
  int player2_movedir = 0;
  do {
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
      SDL_Keycode keycode;
      switch (event.type)
      {
        case SDL_KEYUP:
                          keycode = event.key.keysym.sym;
                          switch (keycode)
                          {
                            case PLAYER1_MOVEUP:
                              player1_movedir = 0;
                              break;
                            case PLAYER1_MOVEDOWN:
                              player1_movedir = 0;
                              break;
                          }
                          break;
        case SDL_KEYDOWN: 
                          keycode = event.key.keysym.sym;
                          switch (keycode)
                          {
                            case PLAYER1_MOVEUP:
                              player1_movedir = 1;
                              break;
                            case PLAYER1_MOVEDOWN:
                              player1_movedir = -1;
                              break;
                          }
                          break;
        case SDL_QUIT: 
                          quit = true;
                          break;
        
      }
    }

    //TODO: draw all the pong stuff

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    player2_movedir = CalcPlayer2Move(gamestate);
    next_frame(gamestate, player1_movedir, player2_movedir);
  } while(!quit);
  return 0;
}

float rand_speed()
{
  int random_number = rand();
  int max_rand = 20;
  random_number %= max_rand;
  random_number -= max_rand/2;
  return ((float) random_number) / 100.0f;
}

GameState* InitGameState()
{
  GameState* gamestate = malloc(sizeof(gamestate));

  if (gamestate == NULL)
  {
    fprintf(stderr, "Failed to init gamestate\n");
    exit(2);
  }

  gamestate->ball = malloc(sizeof(Ball));
  if (gamestate->ball == NULL)
  {
    fprintf(stderr, "Failed to init gamestate's ball\n");
    exit(2);
  }
  gamestate->ball->dx = rand_speed();
  gamestate->ball->dy = rand_speed();
  gamestate->ball->x = 0;
  gamestate->ball->y = 0;

  gamestate->player1 = malloc(sizeof(NormRect));
  if (gamestate->player1 == NULL)
  {
    fprintf(stderr, "Failed to init gamestate's player1\n");
    exit(2);
  }
  gamestate->player1->x = -1;
  gamestate->player1->y = PADDLE_LENGTH / 2.0f;
  gamestate->player1->w = PADDLE_WIDTH;
  gamestate->player1->h = PADDLE_LENGTH;

  gamestate->player2 = malloc(sizeof(NormRect));
  if (gamestate->player2 == NULL)
  {
    fprintf(stderr, "Failed to init gamestate's player2\n");
    exit(2);
  }
  gamestate->player2->x = 1 - PADDLE_WIDTH;
  gamestate->player2->y = PADDLE_LENGTH / 2.0f;
  gamestate->player2->w = PADDLE_WIDTH;
  gamestate->player2->h = PADDLE_LENGTH;

  return gamestate;
}

void next_frame(GameState* gamestate, int player1_move_dir, int player2_move_dir)
{
  // Update paddle pos
  gamestate->player1->y += PADDLE_SPEED * player1_move_dir;
  gamestate->player2->y += PADDLE_SPEED * player2_move_dir;

  // Update ball pos
  gamestate->ball->x += gamestate->ball->dx;
  gamestate->ball->y += gamestate->ball->dy;

  // Collision detection: ball and paddles 
  // TODO

  // Collision detection: ball and walls
  if (gamestate->ball->y >= (1 + BALL_RADIUS))
  {
    gamestate->ball->dy = -gamestate->ball->dy;
    gamestate->ball->y = 1 - BALL_RADIUS;
  }
  else if (gamestate->ball->y <= -(1 + BALL_RADIUS))
  {
    gamestate->ball->dy = -gamestate->ball->dy;
    gamestate->ball->y = -1 + BALL_RADIUS;
  }

  // Collision detection: ball and goal
  if (gamestate->ball->x >= (1 + BALL_RADIUS))
  {
    gamestate->score[0]++;
    reset_ball(gamestate);
  }
  else if (gamestate->ball->x <= (-1 - BALL_RADIUS))
  {
    gamestate->score[1]++;
    reset_ball(gamestate);
  }
}

void reset_ball(GameState* gamestate)
{
  gamestate->ball->x = 0;
  gamestate->ball->dy = rand_speed();
  gamestate->ball->dx = rand_speed();
}

int CalcPlayer2Move(GameState* gamestate)
{
  return 0;
}
