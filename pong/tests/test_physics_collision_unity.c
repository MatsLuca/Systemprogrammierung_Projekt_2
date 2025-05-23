/* ------------------------------------------------------------------
 * test_physics_collision_unity.c - Unity-Tests für Ball-Kollisionen & Punkte
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include "unity.h"
#include "physics.h"
#include "config.h"

/* Diese Tests prüfen Ball-Kollisionen, Punktevergabe und Spielende */

// Vorbereitung vor jedem Test
void setUp(void) {}
// Aufräumen nach jedem Test
void tearDown(void) {}

// Hilfsfunktion: Ballposition und -geschwindigkeit setzen
typedef struct { float x, y, vx, vy; } BallInit;
static void init_ball(game_state_t *game, BallInit b) {
    game->ball.x  = b.x;
    game->ball.y  = b.y;
    game->ball.vx = b.vx;
    game->ball.vy = b.vy;
}

// Ball-Kollisionen und Punkte-Tests

// Prüft Reflexion an linker Seitenwand
void test_side_wall_reflection_left(void) {
    game_state_t game = physics_create_game(80, 24);
    init_ball(&game, (BallInit){ .x = -1, .y = 12, .vx = -1.0f, .vy = 0.0f });
    physics_update_ball(&game);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, game.ball.vx);
}

// Prüft Reflexion an rechter Seitenwand
void test_side_wall_reflection_right(void) {
    game_state_t game = physics_create_game(80, 24);
    init_ball(&game, (BallInit){ .x = 80, .y = 12, .vx = 1.5f, .vy = 0.0f });
    physics_update_ball(&game);
    TEST_ASSERT_TRUE(game.ball.vx < 0);
}

// Prüft, ob Paddle-Kollision Ball beschleunigt
void test_player_paddle_collision_speedup(void) {
    game_state_t game = physics_create_game(80, 24);
    // Place ball just above player, moving down
    game.player.x     = 30;
    game.player.width = 10;
    game.player.y     = 22;
    init_ball(&game, (BallInit){ .x = 35, .y = 21, .vx = 0.5f, .vy = 1.0f });
    float old_vy = game.ball.vy;
    physics_update_ball(&game);
    TEST_ASSERT_TRUE(fabsf(game.ball.vy) > fabsf(old_vy));
}

// Prüft Punktezählung und Ball-Zurücksetzung
void test_scoring_and_reset(void) {
    game_state_t game = physics_create_game(80, 24);
    init_ball(&game, (BallInit){ .x = 40, .y = -1, .vx = 1.0f, .vy = -1.0f });
    int old_score = game.score;
    physics_update_ball(&game);
    TEST_ASSERT_EQUAL_INT(old_score + 1, game.score);
    // Ball repositioned to center
    TEST_ASSERT_EQUAL_FLOAT(40.0f, game.ball.x);
}

// Prüft Rückgabe false bei Spielende
void test_game_over_return_false(void) {
    game_state_t game = physics_create_game(80, 24);
    init_ball(&game, (BallInit){ .x = 40, .y = 25, .vx = 0.0f, .vy = 1.0f });
    bool result = physics_update_ball(&game);
    TEST_ASSERT_FALSE(result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_side_wall_reflection_left);
    RUN_TEST(test_side_wall_reflection_right);
    RUN_TEST(test_player_paddle_collision_speedup);
    RUN_TEST(test_scoring_and_reset);
    RUN_TEST(test_game_over_return_false);

    return UNITY_END();
}
