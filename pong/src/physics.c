/* ------------------------------------------------------------------
 * physics.c - Ballbewegung und Kollisions-Erkennung
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ncurses.h>
#include <time.h>
#include "physics.h"
#include "config.h"

/* Flag für Flash-Effekt */
int player_flash = 0;
int bot_flash    = 0;

/* Spielzustand initialisieren */
game_state_t physics_create_game(int width, int height)
{
    game_state_t game = {0};

    /* Eingabeparameter prüfen */
    if (width < MIN_TERMINAL_WIDTH || height < MIN_TERMINAL_HEIGHT)
    {
        /* Bei ungültigen Werten Mindestwerte verwenden */
        width = MIN_TERMINAL_WIDTH;
        height = MIN_TERMINAL_HEIGHT;
    }

    game.field_width  = width;
    game.field_height = height;
    game.score = 0;

    /* Spieler-Paddle initialisieren */
    game.player.width = width / PADDLE_WIDTH_RATIO;
    game.player.x     = (width - game.player.width) / 2;
    game.player.y     = height - 2;

    /* Bot-Paddle initialisieren */
    game.bot          = game.player;
    game.bot.y        = 1;

    /* Ball initialisieren */
    game.ball.x  = width  / 2.0f;
    game.ball.y  = height / 2.0f;
    game.ball.vx = BALL_INITIAL_SPEED;
    game.ball.vy = -BALL_INITIAL_SPEED;

    return game;
}

/* Spieler-Paddle bewegen */
void physics_player_move(game_state_t *game, int dx)
{
    game->player.x += dx * PLAYER_PADDLE_SPEED;
    
    /* Paddle in Spielfeld halten */
    if (game->player.x < 0)
    {
        game->player.x = 0;
    }
    if (game->player.x + game->player.width >= game->field_width)
    {
        game->player.x = game->field_width - game->player.width - 1;
    }
}

/* Ball reflektieren und Geschwindigkeit erhöhen */
static void reflect(ball_t *ball)
{
    ball->vy = -ball->vy;
    ball->vx *= BALL_BOUNCE_MULTIPLIER;
    ball->vy *= BALL_BOUNCE_MULTIPLIER;
}

/* Ball mit Standard Geschwindigkeit zentrieren */
static void reset_ball(game_state_t *game, int dir_down)
{
    /* 1. Ball zentrieren */
    game->ball.x = game->field_width  / 2.0f;
    game->ball.y = game->field_height / 2.0f;

    /* 2. Basisgeschwindigkeit abhängig vom Score                */
    float base_speed = BALL_INITIAL_SPEED *
                       (1.0f + game->score * SPEED_PER_POINT);

    /* Optional: Obergrenze, damit es nicht unspielbar wird       */
    if (base_speed > BALL_MAX_SPEED) base_speed = BALL_MAX_SPEED;

    /* 3. Zufällige horizontale Richtung                          */
    game->ball.vx = (rand() & 1) ?  base_speed : -base_speed;

    /* 4. Vertikale Richtung: nach unten (=+), sonst nach oben    */
    game->ball.vy = dir_down ?  base_speed : -base_speed;
}

static void show_countdown(void)
{
    const char *txt[] = {"3","2","1"};
    for (int i = 0; i < 3; ++i) {
        mvprintw(LINES/2, COLS/2 - 1, txt[i]);
        refresh();
        nanosleep(&(struct timespec){0, 400*1000*1000}, NULL); /* 400 ms */
    }
    erase();   /* altes Zeichen Wegwischen */
}

/*  --------------------------------------------------------------
    physics_update_ball_simple  –  bewegt den Ball Schritt für Schritt
    und prüft nach jedem kleinen Schritt auf Kollisionen.
    → Gibt false zurück, wenn das Spiel vorbei ist (Ball unten raus).
    -------------------------------------------------------------- */
bool physics_update_ball(game_state_t *game)
{
    ball_t *ball = &game->ball;

    /* ---------------------------------------------
       1. Wie viele „Mini-Schritte“ brauchen wir?    
          Wir teilen die Bewegung so auf, dass      
          weder dx noch dy größer als 1 Zelle ist.  
       --------------------------------------------- */
    int sub_steps = (int)ceilf(
        fmaxf(fabsf(ball->vx), fabsf(ball->vy))
    );
    if (sub_steps < 1) sub_steps = 1;   /* Sicherheitsnetz */

    /* Schrittweite pro Sub-Step */
    float step_x = ball->vx / sub_steps;
    float step_y = ball->vy / sub_steps;

    /* ---------------------------------------------
       2. Sub-Steps nacheinander abarbeiten          
       --------------------------------------------- */
    for (int s = 0; s < sub_steps; ++s)
    {
        ball->x += step_x;
        ball->y += step_y;

        /* -------- Seitenwände (links / rechts) ---- */
        if (ball->x <= 0 || ball->x >= game->field_width - 1)
        {
            /* Ball horizontal umdrehen               */
            ball->vx = -ball->vx;
            step_x   = -step_x;                       /* Rest korrigieren */
            /* Ball in Spielfeld halten               */
            ball->x = fminf(fmaxf(ball->x, 0),
                           game->field_width - 1);
        }

        /* -------- Bot-Paddle (oben) --------------- */
        if (ball->vy < 0 &&                                    /* Ball geht nach oben  */
            ball->y <= game->bot.y + 1 &&
            ball->y >= game->bot.y &&
            ball->x >= game->bot.x &&
            ball->x <= game->bot.x + game->bot.width)
        {
            reflect(ball);                                     /* Richtung + Speed */

            bot_flash = 4;

            step_y = -fabsf(ball->vy) / (sub_steps - s);       /* neue Schrittgröße */
        }

        /* -------- Spieler-Paddle (unten) ---------- */
        if (ball->vy > 0 &&                                    /* Ball geht nach unten */
            ball->y >= game->player.y - 1 &&
            ball->y <= game->player.y &&
            ball->x >= game->player.x &&
            ball->x <= game->player.x + game->player.width)
        {
            reflect(ball);

            player_flash = 4;

            step_y = -fabsf(ball->vy) / (sub_steps - s);
        }

        /* -------- Punkte & Spielende -------------- */
        if (ball->y < 0)                                      /* oben raus -> Punkt  */
        {
            game->score += 1;
            reset_ball(game, /*dir_down=*/1);
            show_countdown();
            break;                                            /* Frame fertig       */
        }
        else if (ball->y > game->field_height)                /* unten raus -> Ende */
        {
            return false;
        }
    }

    return true;  /* Spiel läuft weiter */
}


