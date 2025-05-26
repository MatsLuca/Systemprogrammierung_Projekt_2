/* ------------------------------------------------------------------
 * test_ai_speed_unity.c - Unity‑Tests für die neue Bot‑Physik
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include "unity.h"
#include "ai.h"
#include "config.h"
#include <math.h>   /* fabsf */

/* Diese Tests prüfen die Beschleunigungs‑Skalierung des Bots
   sowie korrektes Anhalten an den Spielfeldgrenzen               */

void setUp(void) {}
void tearDown(void) {}

/* Hilfsfunktion: erstellt Grundspielzustand */
static game_state_t make_game(int width, int score)
{
    game_state_t g = physics_create_game(width, 24);
    g.score = score;
    g.bot.vx = g.bot.ax = 0.0f;            /* definierter Ausgangszustand */
    g.ball.x = width - 1;                  /* Ball ganz rechts -> Bot muss nach rechts */
    return g;
}

/* 1.   Beschleunigung berücksichtigt Score                        */
void test_ai_accel_includes_score(void)
{
    int score = 5;
    game_state_t g = make_game(100, score);

    ai_update(&g);

    float expected_ax = BOT_BASE_ACCELERATION + score * BOT_ACCEL_PER_POINT;
    /* vx == ax, weil v_start = 0 und genau ein Physik‑Frame vergangen ist */
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, expected_ax, g.bot.vx);
}

/* 2.   Basisbeschleunigung bei Score 0                             */
void test_ai_accel_base_speed(void)
{
    game_state_t g = make_game(100, 0);

    ai_update(&g);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, BOT_BASE_ACCELERATION, g.bot.vx);
}

/* 3.   Bot stoppt korrekt an der rechten Spielfeldgrenze           */
void test_ai_boundary_right_stop(void)
{
    int width = 50;
    game_state_t g = physics_create_game(width, 10);
    /* Bot fast am rechten Rand platzieren                           */
    g.bot.x = width - g.bot.width - 2;
    g.bot.vx = g.bot.ax = 0.0f;
    g.ball.x = width - 1;      /* Ball rechts -> Richtung +1        */

    ai_update(&g);

    int max_x = width - g.bot.width - 1;
    TEST_ASSERT_EQUAL_INT(max_x, (int)roundf(g.bot.x));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, g.bot.vx);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ai_accel_includes_score);
    RUN_TEST(test_ai_accel_base_speed);
    RUN_TEST(test_ai_boundary_right_stop);

    return UNITY_END();
}
