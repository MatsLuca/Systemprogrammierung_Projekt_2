/* ------------------------------------------------------------------
 * render.c - Darstellung mit ncurses
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include "render.h"
#include <math.h>
#include "config.h"   /* BOT_INITIAL_SPEED … */

extern int player_flash;
extern int bot_flash;

/* Aktuell keine zusätzliche Initialisierung nötig, Farben werden in main gesetzt */
void render_init(void)
{
    /* leer */
}

/* Fette, einfarbige Schläger */
static void draw_paddle(const paddle_t *p, int color, int flash)
{
    if (flash) attron(A_REVERSE);
    attron(COLOR_PAIR(color));
    for (int i = 0; i < p->width; ++i)
        mvaddch(p->y, p->x + i, ACS_BLOCK);
    attroff(COLOR_PAIR(color));
    if (flash) attroff(A_REVERSE);
}

void render_frame(const game_state_t *g)
{
    erase();

    /* 1.  Spielfeld-Rahmen (vollständig) ---------------------------- */
    attron(A_DIM);

    /* obere Kante mit Ecken */
    mvaddch(g->bot.y - 1,               0,                 ACS_ULCORNER);
    mvhline(g->bot.y - 1,               1,                 ACS_HLINE, g->field_width - 2);
    mvaddch(g->bot.y - 1, g->field_width - 1,              ACS_URCORNER);

    /* untere Kante mit Ecken */
    mvaddch(g->player.y + 1,            0,                 ACS_LLCORNER);
    mvhline(g->player.y + 1,            1,                 ACS_HLINE, g->field_width - 2);
    mvaddch(g->player.y + 1, g->field_width - 1,           ACS_LRCORNER);

     /* linke und rechte Seiten */
    int inner_height = g->player.y - g->bot.y + 1;   /* +1: bis zur Bottom-Line */
    mvvline(g->bot.y, 0,                                    ACS_VLINE, inner_height);   /* Zeilen dazwischen*/
    mvvline(g->bot.y, g->field_width-1,                     ACS_VLINE, inner_height);

    attroff(A_DIM);

    /* 2.  Score-Zeile ---------------------------------------------- */
    attron(COLOR_PAIR(5) | A_BOLD);
    mvprintw(0, 2, "Score: %d   (q = quit)", g->score);
    attroff(COLOR_PAIR(5) | A_BOLD);

    /* 2a. Bot- & Ball-Geschwindigkeit ------------------------------- */
    int bot_step  = BOT_INITIAL_SPEED + g->score * BOT_SCORE_SPEED_INCREMENT;
    float ball_sp = sqrtf(g->ball.vx * g->ball.vx + g->ball.vy * g->ball.vy);

    /* Wir bauen die Zeile Stück für Stück, um die Werte einfärben zu können */
    int col = g->field_width - 25;   /* rechter Rand wie gehabt */

    mvprintw(0, col, "Bot:");
    attron(COLOR_PAIR(6) | A_BOLD);
    printw("%3d", bot_step);
    attroff(COLOR_PAIR(6) | A_BOLD);

    printw("  Ball:  ");
    attron(COLOR_PAIR(7) | A_BOLD);
    printw("%4.2f", ball_sp);
    attroff(COLOR_PAIR(7) | A_BOLD);

    /* 3.  Spielobjekte --------------------------------------------- */
    draw_paddle(&g->player, 3, player_flash > 0);
    draw_paddle(&g->bot,    4, bot_flash    > 0);
    if (player_flash > 0) player_flash--;
    if (bot_flash    > 0) bot_flash--;

    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddch((int)g->ball.y, (int)g->ball.x, ACS_DIAMOND);
    attroff(COLOR_PAIR(2) | A_BOLD);

    refresh();
}

