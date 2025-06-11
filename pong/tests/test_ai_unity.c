/* ------------------------------------------------------------------
 * test_ai_unity.c - Unity-Tests für das KI-Modul
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#include "unity.h"
#include "physics.h"
#include "ai.h"
#include "config.h"

/* Diese Tests prüfen das Bewegungsverhalten der KI unter verschiedenen Bedingungen */

void setUp(void) {}
void tearDown(void) {}

/* ------------------------------------------------------------------
 * make_game
 * Erstellt einen Spielzustand mit definierter Ball- und Bot-Position.
 *
 * Parameter:
 *   ball_x      – X‑Position des Balls
 *   bot_x       – Start‑X‑Position des Bot‑Paddles
 *   field_width – Spielfeldbreite
 *
 * Rückgabe:
 *   game_state_t mit vorkonfigurierten Werten
 * ------------------------------------------------------------------ */
static game_state_t make_game(float ball_x, float bot_x, int field_width) {
    game_state_t game = physics_create_game(field_width, 24);
    game.ball.x = ball_x;
    game.bot.x  = bot_x;
    game.field_width = field_width;
    return game;
}

/* Prüft, ob der Bot sich nach rechts bewegt, wenn der Ball rechts ist */
void test_ai_moves_bot_right_when_ball_is_right(void) {
    game_state_t game = make_game(50.0f, 10, 100);
    ai_update(&game);
    TEST_ASSERT_TRUE(game.bot.x > 10);
}

/* Prüft, ob der Bot sich nach links bewegt, wenn der Ball links ist */
void test_ai_moves_bot_left_when_ball_is_left(void) {
    game_state_t game = make_game(10.0f, 50, 100);
    ai_update(&game);
    TEST_ASSERT_TRUE(game.bot.x < 50);
}

/* Prüft, dass der Bot am linken Spielfeldrand bleibt */
void test_ai_not_beyond_left_boundary(void) {
    game_state_t game = make_game(0.0f, 0, 100);
    ai_update(&game);
    TEST_ASSERT_EQUAL_INT(0, game.bot.x);
}

/* Prüft, dass der Bot am rechten Spielfeldrand bleibt */
void test_ai_not_beyond_right_boundary(void) {
    int width = 100;
    game_state_t game = make_game(width - 1, width - (width / PADDLE_WIDTH_RATIO) - 1, width);
    ai_update(&game);
    int max_x = width - game.bot.width - 1;
    TEST_ASSERT_EQUAL_INT(max_x, game.bot.x);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_ai_moves_bot_right_when_ball_is_right);
    RUN_TEST(test_ai_moves_bot_left_when_ball_is_left);
    RUN_TEST(test_ai_not_beyond_left_boundary);
    RUN_TEST(test_ai_not_beyond_right_boundary);

    return UNITY_END();
}

