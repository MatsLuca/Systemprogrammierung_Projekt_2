/* ------------------------------------------------------------------
 * physics.h - Header des Physik-Moduls
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */


#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>
#include "config.h"

/* Spielkonstanten wanderten nach config.h */

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

/* ---------------------------------------------------------------
 * Ereignisse aus dem Physik-Update, um UI zu entkoppeln
 * Bitmaske erlaubt Mehrfachereignisse pro Frame
 * --------------------------------------------------------------- */
typedef enum {
    PHYS_EVENT_NONE          = 0,
    PHYS_EVENT_HIT_PLAYER    = 1 << 0,
    PHYS_EVENT_HIT_BOT       = 1 << 1,
    PHYS_EVENT_SCORED        = 1 << 2,
    PHYS_EVENT_GAME_OVER     = 1 << 3,
} physics_event_t;

/* RNG-Injektion für testbare/konfigurierbare Zufallswerte */
void physics_seed(unsigned int seed);
void physics_set_random_provider(unsigned int (*rand_func)(void));

game_state_t physics_create_game(int width, int height);
/* Rückwärtskompatibel: true=weiter, false=Game Over */
bool physics_update_ball(game_state_t *game);
/* Neue API: liefert Event-Bitmaske dieses Updates */
physics_event_t physics_update_ball_events(game_state_t *game);
void physics_player_update(game_state_t *g, int input_dx);
void update_paddle(paddle_t *p,
                   float dir,
                   float accel,
                   float vmax,
                   int field_w);

#endif /* PHYSICS_H */
