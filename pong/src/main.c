/* ------------------------------------------------------------------
 * main.c - Hauptprogramm und Spielschleife für Console Pong
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "input.h"
#include "ai.h"
#include "physics.h"
#include "render.h"
#include "cleanup.h"

#define TICK_RATE_MS 100    /* ~10 FPS für flüssiges Spiel */

static void sleep_ms(unsigned int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    /* ncurses initialisieren */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    /* Terminalgröße prüfen */
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    if (max_x < MIN_TERMINAL_WIDTH || max_y < MIN_TERMINAL_HEIGHT)
    {
        endwin();
        printf("Terminal too small! Need at least %dx%d\n", 
               MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
        return EXIT_FAILURE;
    }

    /* Spielsysteme initialisieren */
    input_init();
    render_init();

    game_state_t game = physics_create_game(max_x, max_y, DIFFICULTY_EASY);

    /* Haupt-Spielschleife */
    bool running = true;
    while (running)
    {
        /* Eingabe verarbeiten */
        input_action_t action = input_poll();
        if (action.quit)
        {
            running = false;
            break;
        }
        physics_player_move(&game, action.dx);

        /* KI aktualisieren */
        ai_update(&game);

        /* Physik aktualisieren */
        if (!physics_update_ball(&game))
        {
            running = false;
        }

        /* Frame anzeigen */
        render_frame(&game);

        sleep_ms(TICK_RATE_MS);
    }

    /* Spielende anzeigen */
    nodelay(stdscr, FALSE);
    mvprintw(game.field_height / 2, 2, "Game over - press any key");
    refresh();
    getch();

    cleanup_ncurses();

    return EXIT_SUCCESS;
}
