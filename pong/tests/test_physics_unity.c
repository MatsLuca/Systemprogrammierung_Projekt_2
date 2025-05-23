/* ------------------------------------------------------------------
 * test_physics_unity.c - Unity-Tests für das Physik-Modul
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include "unity.h"
#include "physics.h"


/* Diese Tests prüfen Grundfunktionen des Physik-Moduls */

// Vorbereitung vor jedem Test
void setUp(void) {}

// Aufräumen nach jedem Test
void tearDown(void) {}

// Prüft Initialisierung von Ball-Position und -Geschwindigkeit
void test_physics_ball_creation(void)
{
    game_state_t game = physics_create_game(80, 24, DIFFICULTY_EASY);

    /* Startposition des Balls prüfen */
    TEST_ASSERT_EQUAL_FLOAT(40.0f, game.ball.x);
    TEST_ASSERT_EQUAL_FLOAT(12.0f, game.ball.y);

    /* Startgeschwindigkeit des Balls prüfen */
    TEST_ASSERT_EQUAL_FLOAT(BALL_START_SPEED, game.ball.vx);
    TEST_ASSERT_EQUAL_FLOAT(-BALL_START_SPEED, game.ball.vy);
}

// Prüft Initialisierung der Paddles
void test_physics_paddle_creation(void)
{
    game_state_t game = physics_create_game(80, 24, DIFFICULTY_EASY);

    /* Größe der Paddles prüfen */
    TEST_ASSERT_EQUAL_INT(80 / PADDLE_WIDTH_RATIO, game.player.width);
    TEST_ASSERT_EQUAL_INT(80 / PADDLE_WIDTH_RATIO, game.bot.width);

    /* Position der Paddles prüfen */
    TEST_ASSERT_EQUAL_INT(22, game.player.y);  /* height - 2 */
    TEST_ASSERT_EQUAL_INT(1, game.bot.y);
}

// Prüft Bewegung des Spieler-Paddles
void test_physics_paddle_movement(void)
{
    game_state_t game = physics_create_game(80, 24, DIFFICULTY_EASY);
    int initial_x = game.player.x;

    /* Bewegung nach rechts prüfen */
    physics_player_move(&game, 1);
    TEST_ASSERT_EQUAL_INT(initial_x + PADDLE_SPEED_MULTIPLIER, game.player.x);

    /* Bewegung nach links prüfen */
    physics_player_move(&game, -1);
    TEST_ASSERT_EQUAL_INT(initial_x, game.player.x);
}

// Prüft Behandlung der Spielfeld-Randbedingungen
void test_physics_boundary_conditions(void)
{
    game_state_t game = physics_create_game(20, 10, DIFFICULTY_EASY);

    /* Mindestgröße des Spielfelds prüfen */
    TEST_ASSERT_TRUE(game.field_width >= MIN_TERMINAL_WIDTH);
    TEST_ASSERT_TRUE(game.field_height >= MIN_TERMINAL_HEIGHT);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_physics_ball_creation);
    RUN_TEST(test_physics_paddle_creation);
    RUN_TEST(test_physics_paddle_movement);
    RUN_TEST(test_physics_boundary_conditions);

    return UNITY_END();
}

