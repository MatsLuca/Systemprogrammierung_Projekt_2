/* ------------------------------------------------------------------
 * input.h - Header für die Tastatureingabe
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#ifndef INPUT_H
#define INPUT_H

typedef struct
{
    int dx;      /* -1 links, +1 rechts, 0 keine */
    int quit;    /* ungleich 0, wenn Benutzer abbrechen möchte */
} input_action_t;

void input_init(void);
input_action_t input_poll(void);

#endif /* INPUT_H */
