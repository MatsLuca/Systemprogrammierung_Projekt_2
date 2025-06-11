/* ------------------------------------------------------------------
 * ai.c - Bot-Logik: Geschwindigkeit an Schwierigkeitsgrad anpassen
 * Copyright 2025 Hochschule Hannover
* Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#include "ai.h"
#include "config.h"
#include <math.h>     

/* ------------------------------------------------------------------
 * ai_update
 * Aktualisiert die Position und Beschleunigung des Bot‑Paddles,
 * damit es dem Ball folgt.
 *
 * Parameter:
 *   g – Zeiger auf den aktuellen Spielzustand.
 *
 * Rückgabe:
 *   keine (Spielzustand wird in‑place modifiziert)
 * ------------------------------------------------------------------ */
void ai_update(game_state_t *g)
{
    /* x-Koordinate der Ballmitte und des Bot-Mittelpunkts            */
    float ball_mid = g->ball.x;
    float bot_mid  = g->bot.x + g->bot.width / 2.0f;

    float dir = 0.0f;
    if (fabsf(ball_mid - bot_mid) > 0.5f)
        dir = (ball_mid > bot_mid) ? +1.0f : -1.0f;

    float accel = BOT_BASE_ACCELERATION +
                  BOT_ACCEL_PER_POINT * g->score;

    update_paddle(&g->bot, dir,
                  accel,
                  BOT_MAX_SPEED,
                  g->field_width);
}
