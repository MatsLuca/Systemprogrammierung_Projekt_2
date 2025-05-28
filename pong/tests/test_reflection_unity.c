/* ------------------------------------------------------------------
 *  test_reflection_unity.c – Tests für Offset-Reflexion & Spin
 * ------------------------------------------------------------------ */
#include "unity.h"
#include "physics.h"
#include "config.h"
#include <math.h>

void setUp(void)  {}
void tearDown(void) {}

/* Hilfs-Setup: Ball genau auf Höhe des Spieler-Paddles platzieren */
static void place_ball(game_state_t *g, float rel_x, float paddle_vx)
{
    /* rel_x:  −1 = ganz links auf dem Paddle,  +1 = ganz rechts   */
    g->player.vx = paddle_vx;

    float px = g->player.x + (1 + rel_x) * g->player.width / 2.0f;
    g->ball.x  = px;
    g->ball.y  = g->player.y - 1;
    g->ball.vx = 0.0f;
    g->ball.vy = 1.0f;        /* bewegt sich nach unten → trifft Paddle */
}

/* ---------- 1. Anschnibbeln ------------------------------------ */
void test_offset_reflection_direction(void)
{
    game_state_t g = physics_create_game(80,24);

    /* Ganz rechts treffen (Offset ≈ +1) → vx nach rechts (>0)     */
    place_ball(&g, +1.0f, 0.0f);
    physics_update_ball(&g);
    TEST_ASSERT_TRUE(g.ball.vx > 0);

    /* Ganz links treffen (Offset ≈ −1) → vx nach links (<0)        */
    place_ball(&g, -1.0f, 0.0f);
    physics_update_ball(&g);
    TEST_ASSERT_TRUE(g.ball.vx < 0);
}

void test_offset_reflection_magnitude(void)
{
    game_state_t g = physics_create_game(80,24);

    /* Mitte vs. Außen: Außen muss |vx| größer sein                 */
    place_ball(&g,  0.0f, 0.0f);      /* mittig */
    physics_update_ball(&g);
    float v_mid = fabsf(g.ball.vx);

    place_ball(&g,  0.9f, 0.0f);      /* weit außen */
    physics_update_ball(&g);
    float v_edge = fabsf(g.ball.vx);

    TEST_ASSERT_TRUE(v_edge > v_mid + 1e-3);
}

/* ---------- 2. Spin-Übertrag ----------------------------------- */
void test_spin_transfer(void)
{
    game_state_t g = physics_create_game(80,24);

    /* Ohne Offset, Paddle streicht nach rechts mit vx = +6         */
    place_ball(&g, 0.0f, 6.0f);
    physics_update_ball(&g);

    /* Erwartung: vx ≈ 6 * 0.30 * BOUNCE  (vor evtl. Deckelung)     */
    float expect = 6.0f * 0.30f * (BALL_BOUNCE_MULTIPLIER);
    float got    = g.ball.vx;

    /* Toleranz wegen Mindest-/Max-Speed-Clamps                     */
    TEST_ASSERT_FLOAT_WITHIN(0.2f, expect, got);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_offset_reflection_direction);
    RUN_TEST(test_offset_reflection_magnitude);
    RUN_TEST(test_spin_transfer);
    return UNITY_END();
}
