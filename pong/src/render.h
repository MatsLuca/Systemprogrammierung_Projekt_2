/* ------------------------------------------------------------------
 * render.h - Header des Darstellungs-Moduls
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */


#ifndef RENDER_H
#define RENDER_H

#include "physics.h"

void render_init(void);
void render_frame(const game_state_t *game, physics_event_t events);

/* Optionaler Render-Countdown (UI, nicht Physik) */
void render_countdown(void);

#endif /* RENDER_H */
