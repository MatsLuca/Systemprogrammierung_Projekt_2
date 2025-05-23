/* ------------------------------------------------------------------
 * render.h - Header des Darstellungs-Moduls
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */


#ifndef RENDER_H
#define RENDER_H

#include "physics.h"

void render_init(void);
void render_frame(const game_state_t *game);

#endif /* RENDER_H */
