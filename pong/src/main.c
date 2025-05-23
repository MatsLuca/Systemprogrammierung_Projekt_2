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
#include <locale.h>

#include "input.h"
#include "ai.h"
#include "physics.h"
#include "render.h"
#include "cleanup.h"
#include "config.h"

#define PHYSICS_DT_MS 100   /* Ball‑Physik nur alle 100 ms */
#define RENDER_DT_MS   16   /* ~60 FPS für flüssiges Bild  */

static void sleep_ms(unsigned int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/* Aktuelle Laufzeit in Millisekunden (monotonic clock) */
static unsigned long ms_now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000UL + ts.tv_nsec / 1000000UL);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");   /* Unicode aktivieren */
    (void)argc;
    (void)argv;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    /* Farben, falls Terminal das kann */
    if (has_colors()) {
        start_color();
        use_default_colors();          /* –1 = Term-Hintergrund */
        init_pair(1, COLOR_WHITE,  -1);   /* Standardtext   */
        init_pair(2, COLOR_CYAN,   -1);   /* Ball           */
        init_pair(3, COLOR_YELLOW, -1);   /* Spieler-Paddle */
        init_pair(4, COLOR_MAGENTA,-1);   /* Bot-Paddle     */
        init_pair(5, COLOR_GREEN,  -1);   /* Score-Zeile    */
    }

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

    game_state_t game = physics_create_game(max_x, max_y);
    unsigned long last_phys = ms_now();

    /* Haupt-Spielschleife */
    bool running = true;
    while (running)
    {
        /* Eingabe verarbeiten */
        input_action_t action = input_poll();
        if (action.quit) {
            break;
        }
        physics_player_move(&game, action.dx);

        /* Physik & KI nur alle 100 ms -------------------------------- */
        unsigned long now = ms_now();
        if (now - last_phys >= PHYSICS_DT_MS) {
            ai_update(&game);
            if (!physics_update_ball(&game)) {
                break;
            }
            last_phys = now;
        }

        /* Frame anzeigen (60 FPS) ------------------------------------ */
        render_frame(&game);
        sleep_ms(RENDER_DT_MS);
    }

    /* Spielende anzeigen */
    nodelay(stdscr, FALSE);
    mvprintw(game.field_height / 2, 2, "Game over - press any key");
    refresh();
    getch();

    cleanup_ncurses();

    return EXIT_SUCCESS;
}
