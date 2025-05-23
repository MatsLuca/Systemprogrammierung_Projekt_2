/*  ----------------------------------------------------------------
 *  test_ball_speed_unity.c  –  Testet, ob der Ball nach einem Punkt
 *  mit korrekter Geschwindigkeit neu startet
 *  ---------------------------------------------------------------- */
#include "unity.h"
#include "physics.h"
#include "config.h"

/* Hilfsfunktion: einen Punkt erzwingen,
   indem wir den Ball über das obere Spielfeld hinausschicken       */
static void force_score(game_state_t *g)
{
    /* Ball eine Zeile „oberhalb“ platzieren, nach oben fliegend    */
    g->ball.x  = g->field_width / 2;
    g->ball.y  = -1;            /* y < 0  → Punkt für Spieler       */
    g->ball.vx = 0.0f;
    g->ball.vy = -1.0f;         /* Richtung egal, Hauptsache < 0    */
    physics_update_ball(g);     /* löst reset_ball() intern aus     */
}

/*  Erwartung:  v = BALL_INITIAL_SPEED * (1 + score * SPEED_PER_POINT)
    (ggf. gedeckelt durch BALL_MAX_SPEED)                            */
void test_speed_after_score_scaling(void)
{
    game_state_t g = physics_create_game(80, 24);

    /* Spieler hat schon 4 Punkte                                  */
    g.score = 4;

    force_score(&g);            /* macht Punkt 5, ruft reset_ball   */

    float expected = BALL_INITIAL_SPEED * (1 + 5 * SPEED_PER_POINT);
    if (expected > BALL_MAX_SPEED)
        expected = BALL_MAX_SPEED;

    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected, fabsf(g.ball.vx));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_speed_after_score_scaling);
    return UNITY_END();
}
