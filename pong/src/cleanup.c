/* ------------------------------------------------------------------
 * cleanup.c - Aufräumen des ncurses-Modus
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include "cleanup.h"

/* ------------------------------------------------------------------
 * cleanup_ncurses
 * Beendet den ncurses-Modus und stellt den Terminalzustand wieder her.
 *
 * Parameter:
 *   keine
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
void cleanup_ncurses(void)
{
    endwin();
}
