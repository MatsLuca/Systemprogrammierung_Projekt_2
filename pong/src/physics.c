// Ball movement and collision detection
#include <stdlib.h>
#include <stdbool.h>
#include "physics.h"

static float speed_factor(difficulty_t diff)
{
    switch (diff)
    {
    case DIFFICULTY_EASY:
        return 1.05f;
    case DIFFICULTY_MEDIUM:
        return 1.08f;
    case DIFFICULTY_HARD:
    default:
        return 1.12f;
    }
}

game_state_t physics_create_game(int width, int height, difficulty_t diff)
{
    game_state_t game = {0};
    game.field_width = width;
    game.field_height = height;
    game.difficulty = diff;

    game.player.width = width / 6;
    game.player.x = (width - game.player.width) / 2;
    game.player.y = height - 2;

    game.bot = game.player;
    game.bot.y = 1;

    game.ball.x = width / 2.0f;
    game.ball.y = height / 2.0f;
    /* Ball startet zum Spieler, damit man erst einmal reagieren kann */
    game.ball.vx = 1.0f;            /* oder per rand() nach links/rechts */
    game.ball.vy = 1.0f;            /* fliegt nach unten */

    return game;
}

void physics_player_move(game_state_t *game, int dx)
{
    game->player.x += dx * 2; /* Player paddle faster */
    if (game->player.x < 0)
    {
        game->player.x = 0;
    }
    if (game->player.x + game->player.width >= game->field_width)
    {
        game->player.x = game->field_width - game->player.width - 1;
    }
}

static void reflect(ball_t *ball, float factor)
{
    ball->vy = -ball->vy;
    ball->vx *= factor;
    ball->vy *= factor;
}

bool physics_update_ball(game_state_t *game)
{
    ball_t *b = &game->ball;
    b->x += b->vx;
    b->y += b->vy;

    /* Bounce off side walls */
    if (b->x <= 0 || b->x >= game->field_width - 1)
    {
        b->vx = -b->vx;
    }

    /* Top paddle collision (bot) */
    if ((int)b->y == game->bot.y + 1 &&
        b->x >= game->bot.x &&
        b->x <= game->bot.x + game->bot.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }

    /* Bottom paddle collision (player) */
    if ((int)b->y == game->player.y - 1 &&
        b->x >= game->player.x &&
        b->x <= game->player.x + game->player.width)
    {
        reflect(b, speed_factor(game->difficulty));
    }

    /* Check game over */
    if (b->y < 0 || b->y > game->field_height)
    {
        return false;
    }
    return true;
}
