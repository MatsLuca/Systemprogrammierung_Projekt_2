/* ------------------------------------------------------------------
 * ai.c - Bot-Logik: Geschwindigkeit an Schwierigkeitsgrad anpassen
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include "ai.h"

void ai_update(game_state_t *game)
{
    int base_speed = game->difficulty + BOT_BASE_SPEED;
    int step = base_speed + game->score; /* Bot wird mit Punktestand schneller */
    
    /* Bot bewegt sich zum Ball */
    if (game->ball.x < game->bot.x)
    {
        game->bot.x -= step;
        if (game->bot.x < 0)
        {
            game->bot.x = 0;
        }
    }
    else if (game->ball.x > game->bot.x + game->bot.width)
    {
        game->bot.x += step;
        if (game->bot.x + game->bot.width >= game->field_width)
        {
            game->bot.x = game->field_width - game->bot.width - 1;
        }
    }
}
