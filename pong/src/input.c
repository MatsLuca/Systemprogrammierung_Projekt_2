/* ------------------------------------------------------------------
 * input.c - Nicht-blockierende Tastatureingabe mit ncurses
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */


#include <ncurses.h>
#include "input.h"

/* ------------------------------------------------------------------
 * input_init
 * Initialisiert das Eingabemodul. Momentan sind keine zusätzlichen
 * Schritte nötig, der Platzhalter ermöglicht jedoch spätere Erweiterung.
 *
 * Parameter:
 *   keine
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
void input_init(void)
{
    /* Bislang keine zusätzliche Initialisierung nötig */
}

/* ------------------------------------------------------------------
 * input_poll
 * Liest nicht-blockierend eine Taste aus dem Terminal und übersetzt
 * sie in eine Input‑Aktion für das Spiel.
 *
 * Parameter:
 *   keine
 *
 * Rückgabe:
 *   input_action_t – Struktur mit dx-Bewegung (-1/0/+1) und
 *                   Quit-Flag (1 = Spiel beenden).
 * ------------------------------------------------------------------ */
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
