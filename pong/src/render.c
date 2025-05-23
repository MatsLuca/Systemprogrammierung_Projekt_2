// Rendering using ncurses
#include <ncurses.h>
#include "render.h"

static void draw_paddle(const paddle_t *paddle)
{
    for (int i = 0; i < paddle->width; ++i)
    {
        mvaddch(paddle->y, paddle->x + i, '=');
    }
}

void render_init(void)
{
    /* Colors not required */
}

void render_frame(const game_state_t *game)
{
    erase();
    /* Draw paddles */
    draw_paddle(&game->player);
    draw_paddle(&game->bot);

    /* Draw ball */
    mvaddch((int)game->ball.y, (int)game->ball.x, 'O');

    /* Draw UI line */
    mvprintw(0, 2, "Console Pong - q to quit");

    refresh();
}
