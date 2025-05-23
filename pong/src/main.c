// Copyright 2025 Hochschule Hannover
// Author: ChatGPT
//
// Entry point and main game loop for Console Pong

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "ai.h"
#include "physics.h"
#include "render.h"
#include "cleanup.h"      /* ←  DAS HIER MUSS DRIN STEHEN! */

#define TICK_RATE_MS 100   /* statt 16 – ergibt ~20 FPS */


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

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    input_init();
    render_init();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    game_state_t game = physics_create_game(max_x, max_y, DIFFICULTY_EASY);

    bool running = true;
    while (running)
    {
        /* Input */
        input_action_t action = input_poll();
        if (action.quit)
        {
            running = false;
            break;
        }
        physics_player_move(&game, action.dx);

        /* AI */
        ai_update(&game);

        /* Physics */
        if (!physics_update_ball(&game))
        {
            running = false;
        }

        /* Render */
        render_frame(&game);

        sleep_ms(TICK_RATE_MS);
    }

    cleanup_ncurses();


    mvprintw(game.field_height / 2, 2, "Game over – press any key");
    nodelay(stdscr, FALSE);
    getch();

    return EXIT_SUCCESS;
}
