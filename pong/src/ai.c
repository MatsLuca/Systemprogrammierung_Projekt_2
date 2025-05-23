// Bot logic adapting movement speed to difficulty level
#include "ai.h"

void ai_update(game_state_t *game)
{
    int step = game->difficulty + 1; /* 1, 2 or 3 cells per frame */
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
