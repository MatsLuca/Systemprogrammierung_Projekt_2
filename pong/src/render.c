/* ------------------------------------------------------------------
 * render.c - Darstellung mit ncurses
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

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
    /* Farben hier nicht benötigt */
}

void render_frame(const game_state_t *game)
{
    erase();

    /* Punktestand und Steuerung anzeigen */
    mvprintw(0, 2, "Score: %d   (q = quit)", game->score);

    /* Spielobjekte zeichnen */
    draw_paddle(&game->player);
    draw_paddle(&game->bot);
    mvaddch((int)game->ball.y, (int)game->ball.x, 'O');

    refresh();
}
