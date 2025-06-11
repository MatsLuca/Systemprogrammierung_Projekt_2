/* ------------------------------------------------------------------
 *  test_speed_progress_unity.c
 *  Prüft die progressive Temposteigerung nach Paddle-Hits
 *  Copyright 2025 Hochschule Hannover
 *  Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */
#include "unity.h"
#include "physics.h"
#include "config.h"
#include <math.h>     /* fabsf */

/* ---------- Hilfs-Setup ----------------------------------------- */
void setUp(void)  {}
void tearDown(void) {}

/* ------------------------------------------------------------------
 * simulate_player_hit
 * Platziert den Ball oberhalb des Spieler-Paddles, sodass im
 * anschließenden Physik-Frame garantiert eine Kollision auftritt.
 *
 * Parameter:
 *   g – Zeiger auf Spielzustand
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
static void simulate_player_hit(game_state_t *g)
{
    g->ball.x  = g->player.x + g->player.width / 2.0f;
    g->ball.y  = g->player.y - 1;
    g->ball.vx = 0.0f;
    g->ball.vy = 1.0f;                 /* nach unten → trifft Spieler */

    /* Ein Physik-Frame reicht für die Kollision                    */
    physics_update_ball(g);
}

/* ---------- Tests ---------------------------------------------- */

/* 1.  Mindesttempo wächst wie konfiguriert                         */
void test_min_speed_scaling(void)
{
    game_state_t g = physics_create_game(80, 24);

    float base_min = BALL_MIN_SPEED;
    float expect;

    /* Fünf Paddle-Hits erzwingen                                   */
    for (int n = 1; n <= 5; ++n)
    {
        simulate_player_hit(&g);

        /* erwartetes dynamisches Minimum in diesem Frame            */
        expect = base_min * (1.0f + n * BALL_MIN_SPEED_INC);
        if (expect > BALL_MAX_SPEED)
            expect = BALL_MAX_SPEED;

        float speed = sqrtf(g.ball.vx * g.ball.vx +
                            g.ball.vy * g.ball.vy);

        TEST_ASSERT_TRUE_MESSAGE(speed + 1e-4 >= expect,
            "Ballgeschwindigkeit unter dynamischer Mindestgrenze");
    }
}

/* 2.  Bounce-Multiplikator steigert Tempo bei jedem Hit            */
void test_bounce_growth(void)
{
    game_state_t g = physics_create_game(80, 24);

    /* Ball so langsam wie möglich starten                          */
    g.ball.vx = 0.0f;
    g.ball.vy = BALL_MIN_SPEED;

    float last_speed = sqrtf(g.ball.vx * g.ball.vx +
                             g.ball.vy * g.ball.vy);

    /* Drei Paddle-Hits → Geschwindigkeit muss monoton wachsen      */
    for (int n = 0; n < 3; ++n)
    {
        simulate_player_hit(&g);

        float now = sqrtf(g.ball.vx * g.ball.vx +
                          g.ball.vy * g.ball.vy);

        TEST_ASSERT_TRUE_MESSAGE(now > last_speed + 1e-4,
            "Geschwindigkeit wuchs nicht nach Bounce-Increment");
        last_speed = now;
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_min_speed_scaling);
    RUN_TEST(test_bounce_growth);
    return UNITY_END();
}
