/* ------------------------------------------------------------------
 * physics.c - Ballbewegung und Kollisions-Erkennung
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "physics.h"

/* Geschwindigkeits-Faktor nach Schwierigkeit */
static float speed_factor(difficulty_t diff)
{
    switch (diff)
    {
    case DIFFICULTY_EASY:   
        return SPEED_FACTOR_EASY;
    case DIFFICULTY_MEDIUM: 
        return SPEED_FACTOR_MEDIUM;
    case DIFFICULTY_HARD:
    default:                
        return SPEED_FACTOR_HARD;
    }
}

/* Spielzustand initialisieren */
game_state_t physics_create_game(int width, int height, difficulty_t diff)
{
    game_state_t game = {0};

    /* Eingabeparameter prüfen */
    if (width < MIN_TERMINAL_WIDTH || height < MIN_TERMINAL_HEIGHT)
    {
        /* Bei ungültigen Werten Mindestwerte verwenden */
        width = MIN_TERMINAL_WIDTH;
        height = MIN_TERMINAL_HEIGHT;
    }

    game.field_width  = width;
    game.field_height = height;
    game.difficulty   = diff;
    game.score = 0;

    /* Spieler-Paddle initialisieren */
    game.player.width = width / PADDLE_WIDTH_RATIO;
    game.player.x     = (width - game.player.width) / 2;
    game.player.y     = height - 2;

    /* Bot-Paddle initialisieren */
    game.bot          = game.player;
    game.bot.y        = 1;

    /* Ball initialisieren */
    game.ball.x  = width  / 2.0f;
    game.ball.y  = height / 2.0f;
    game.ball.vx = BALL_START_SPEED;
    game.ball.vy = -BALL_START_SPEED;

    return game;
}

/* Spieler-Paddle bewegen */
void physics_player_move(game_state_t *game, int dx)
{
    game->player.x += dx * PADDLE_SPEED_MULTIPLIER;
    
    /* Paddle in Spielfeld halten */
    if (game->player.x < 0)
    {
        game->player.x = 0;
    }
    if (game->player.x + game->player.width >= game->field_width)
    {
        game->player.x = game->field_width - game->player.width - 1;
    }
}

/* Ball reflektieren und Geschwindigkeit erhöhen */
static void reflect(ball_t *ball, float factor)
{
    ball->vy = -ball->vy;
    ball->vx *= factor;
    ball->vy *= factor;
}

/* Ball mit aktueller Geschwindigkeit zentrieren */
static void reset_ball(game_state_t *game, int dir_down)
{
    float current_speed = sqrtf(game->ball.vx * game->ball.vx + 
                               game->ball.vy * game->ball.vy);
    
    game->ball.x  = game->field_width  / 2.0f;
    game->ball.y  = game->field_height / 2.0f;
    game->ball.vx = (rand() % 2 ? current_speed * 0.7f : 
                                 -current_speed * 0.7f);
    game->ball.vy = dir_down ? current_speed * 0.7f : 
                              -current_speed * 0.7f;
}

/* Haupt-Update der Ball-Physik; false bei Spielende */
bool physics_update_ball(game_state_t *game)
{
    ball_t *b = &game->ball;
    
    /* Ballposition aktualisieren */
    b->x += b->vx;
    b->y += b->vy;
    
    /* Seitenwand-Kollisionen behandeln */
    if (b->x <= 0 || b->x >= game->field_width - 1)
    {
        b->vx = -b->vx;
    }
    
    /* Kollision mit Bot-Paddle (Ball nach oben) */
    if (b->vy < 0 &&
        b->y <= game->bot.y + 1 &&
        b->y >= game->bot.y &&
        b->x >= game->bot.x &&
        b->x <= game->bot.x + game->bot.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }
    
    /* Kollision mit Spieler-Paddle (Ball nach unten) */
    if (b->vy > 0 &&
        b->y >= game->player.y - 1 &&
        b->y <= game->player.y &&
        b->x >= game->player.x &&
        b->x <= game->player.x + game->player.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }
    
    /* Obere Grenze (Punkt für Spieler) */
    if (b->y < 0) 
    {
        game->score += 1;
        reset_ball(game, 1);
    }
    /* Untere Grenze (Spielende) */
    else if (b->y > game->field_height) 
    {
        return false;
    }
    
    return true;
}

