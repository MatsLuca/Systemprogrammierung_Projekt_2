/* ------------------------------------------------------------------
 * physics.h - Header des Physik-Moduls
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */


#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>

/* Spielkonstanten */
#define MIN_TERMINAL_WIDTH 20
#define MIN_TERMINAL_HEIGHT 10
#define PADDLE_WIDTH_RATIO 6
#define BALL_START_SPEED 0.8f
#define PADDLE_SPEED_MULTIPLIER 2
#define SPEED_FACTOR_EASY 1.05f
#define SPEED_FACTOR_MEDIUM 1.08f
#define SPEED_FACTOR_HARD 1.12f

typedef enum
{
    DIFFICULTY_EASY = 0,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} difficulty_t;

typedef struct
{
    int x;
    int y;
    int width;
} paddle_t;

typedef struct
{
    float x;
    float y;
    float vx;
    float vy;
} ball_t;

typedef struct
{
    int field_width;
    int field_height;
    difficulty_t difficulty;
    paddle_t player;
    paddle_t bot;
    ball_t ball;
    int score;
} game_state_t;

game_state_t physics_create_game(int width, int height, difficulty_t diff);
void physics_player_move(game_state_t *game, int dx);
bool physics_update_ball(game_state_t *game);

#endif /* PHYSICS_H */
