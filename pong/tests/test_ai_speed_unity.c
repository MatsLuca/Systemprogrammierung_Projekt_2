/* ------------------------------------------------------------------
 * test_ai_speed_unity.c - Unity-Tests für KI-Geschwindigkeitsanpassung
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include "unity.h"
#include "ai.h"
#include "config.h"

/* Diese Tests prüfen die Anpassung der Bot-Geschwindigkeit je nach Schwierigkeitsgrad und Punktestand */

void setUp(void) {}
void tearDown(void) {}

// Prüft, ob der Bot-Schritt den aktuellen Punktestand einbezieht
void test_ai_step_includes_score(void) {
    game_state_t game = physics_create_game(100,20);
    game.bot.x = 0; game.bot.width = 10; game.score = 5;
    game.ball.x = 20;
    ai_update(&game);
    int expected = (BOT_INITIAL_SPEED) + 5;
    TEST_ASSERT_EQUAL_INT(expected, game.bot.x);
}

// Prüft die Schrittweite bei mittlerem Schwierigkeitsgrad ohne Score
void test_ai_difficulty_medium_speed(void) {
    game_state_t game = physics_create_game(100,20);
    game.bot.x = 0; game.bot.width = 10; game.score = 0;
    game.ball.x = 20;
    ai_update(&game);
    TEST_ASSERT_EQUAL_INT(BOT_INITIAL_SPEED, game.bot.x);
}

// Prüft die Schrittweite bei hartem Schwierigkeitsgrad und Randbegrenzung
void test_ai_difficulty_hard_speed_and_boundary(void) {
    game_state_t game = physics_create_game(50,10);
    game.bot.x = 40; game.bot.width = 5; game.field_width = 50;
    game.score = 2;
    game.ball.x = 49;
    ai_update(&game);
    int max_x = game.field_width - game.bot.width - 1;
    TEST_ASSERT_EQUAL_INT(max_x, game.bot.x);
}


int main(void) {
    UNITY_BEGIN();

    // KI-Geschwindigkeitstests
    RUN_TEST(test_ai_step_includes_score);
    RUN_TEST(test_ai_difficulty_medium_speed);
    RUN_TEST(test_ai_difficulty_hard_speed_and_boundary);

    return UNITY_END();
}
