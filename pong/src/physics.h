/* ------------------------------------------------------------------
 * physics.h - Header des Physik-Moduls
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */


#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>
#include "config.h"

/* Spielkonstanten */
#define MIN_TERMINAL_WIDTH 20
#define MIN_TERMINAL_HEIGHT 10
#define PADDLE_WIDTH_RATIO 6

typedef struct
{
    float x;            /* linke Oberkante (Fließkomma fürs Gleiten) */
    int   y;            /* Zeile bleibt ganzzahlig                   */
    int   width;

    float vx;           /* aktuelle Geschwindigkeit                  */
    float ax;           /* aktuelle Beschleunigung                   */
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
    paddle_t player;
    paddle_t bot;
    ball_t   ball;
    int score;
    int paddle_hits;
} game_state_t;

game_state_t physics_create_game(int width, int height);
bool physics_update_ball(game_state_t *game);
void physics_player_update(game_state_t *g, int input_dx);
void update_paddle(paddle_t *p,
                   float dir,
                   float accel,
                   float vmax,
                   int field_w);

#endif /* PHYSICS_H */
