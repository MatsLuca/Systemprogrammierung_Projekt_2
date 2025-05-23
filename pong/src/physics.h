#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>     /* <- NEU */

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
    int score;          /* Punkte des Spielers */

} game_state_t;

game_state_t physics_create_game(int width, int height, difficulty_t diff);
void physics_player_move(game_state_t *game, int dx);
bool physics_update_ball(game_state_t *game);

#endif /* PHYSICS_H */
