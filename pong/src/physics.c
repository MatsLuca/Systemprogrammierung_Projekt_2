/* ------------------------------------------------------------------
 * physics.c – Ballbewegung und Kollisionen
 * ------------------------------------------------------------------
 *  Zwei Fixes:
 *    1. Kollisionen prüfen jetzt per Bereich (>= / <=) statt ==,
 *       damit der Ball auch bei >1 Zeile pro Frame zuverlässig
 *       das Paddle trifft.
 *    2. Bedingung auf Bewegungsrichtung (b->vy < 0 bzw. > 0),
 *       damit nur die relevante Seite reagiert.
 * ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdbool.h>
#include "physics.h"
#include <math.h>        /* <- DAS HIER HINZUFÜGEN */


/* Geschwindigkeitsfaktor je Schwierigkeitsgrad */
static float speed_factor(difficulty_t diff)
{
    switch (diff)
    {
    case DIFFICULTY_EASY:   return 1.05f;
    case DIFFICULTY_MEDIUM: return 1.08f;
    case DIFFICULTY_HARD:
    default:                return 1.12f;
    }
}

/* Spielzustand initialisieren */
game_state_t physics_create_game(int width, int height, difficulty_t diff)
{
    game_state_t game = {0};

    game.field_width  = width;
    game.field_height = height;
    game.difficulty   = diff;

    game.score = 0;       /* in physics_create_game() */

    game.player.width = width / 6;
    game.player.x     = (width - game.player.width) / 2;
    game.player.y     = height - 2;

    game.bot          = game.player;
    game.bot.y        = 1;

    game.ball.x  = width  / 2.0f;
    game.ball.y  = height / 2.0f;
    game.ball.vx = 0.8f;      /* langsamer Start */
    game.ball.vy = -0.8f;

    return game;
}

/* Spieler-Paddle bewegen */
void physics_player_move(game_state_t *game, int dx)
{
    game->player.x += dx * 2;          /* schnelleres Paddle */
    if (game->player.x < 0)
        game->player.x = 0;
    if (game->player.x + game->player.width >= game->field_width)
        game->player.x = game->field_width - game->player.width - 1;
}

/* Richtungsumkehr + Beschleunigung */
static void reflect(ball_t *ball, float factor)
{
    ball->vy = -ball->vy;
    ball->vx *= factor;
    ball->vy *= factor;
}

/* Hilfsfunktion */
static void reset_ball(game_state_t *game, int dir_down)
{
    float current_speed = sqrtf(game->ball.vx * game->ball.vx + game->ball.vy * game->ball.vy);
    
    game->ball.x  = game->field_width  / 2.0f;
    game->ball.y  = game->field_height / 2.0f;
    game->ball.vx = (rand() % 2 ? current_speed * 0.7f : -current_speed * 0.7f);
    game->ball.vy = dir_down ? current_speed * 0.7f : -current_speed * 0.7f;
}

/* Haupt-Update der Ball-Physik; gibt false bei Game-Over zurück */
bool physics_update_ball(game_state_t *game)
{
    ball_t *b = &game->ball;
    
    /* Position fortschreiben */
    b->x += b->vx;
    b->y += b->vy;
    
    /* Seitenwände */
    if (b->x <= 0 || b->x >= game->field_width - 1)
        b->vx = -b->vx;
    
    /* Paddle-Kollisionen */
    if (b->vy < 0 &&
        b->y <= game->bot.y + 1 &&
        b->y >= game->bot.y &&
        b->x >= game->bot.x &&
        b->x <= game->bot.x + game->bot.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }
    
    if (b->vy > 0 &&
        b->y >= game->player.y - 1 &&
        b->y <= game->player.y &&
        b->x >= game->player.x &&
        b->x <= game->player.x + game->player.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }
    
    if (b->y < 0) {
        game->score += 1;
        reset_ball(game, 1);
    }
    else if (b->y > game->field_height) {
        return false;
    }
    
    return true;  // <- DAS FEHLT!
}