/* ------------------------------------------------------------------
 * cleanup.c - Aufräumen des ncurses-Modus
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include "cleanup.h"

void cleanup_ncurses(void)
{
    endwin();
}
