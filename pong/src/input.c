/* ------------------------------------------------------------------
 * input.c - Nicht-blockierende Tastatureingabe mit ncurses
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include "input.h"
#include "config.h"

void input_init(void)
{
    /* Bislang keine zusätzliche Initialisierung nötig */
}

input_action_t input_poll(void)
{
    input_action_t action = {0, 0};
    int ch = getch(); /* Lese gedrückte Taste (non-blocking) */
    
    switch (ch)
    {
    case KEY_LEFT:
        action.dx = -1;
        break;
    case KEY_RIGHT:
        action.dx = 1;
        break;
    case 'q':
    case 'Q':
        action.quit = 1;
        break;
    default:
        break;
    }
    
    return action;
}
