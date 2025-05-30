/* ------------------------------------------------------------------
 * physics.c - Ballbewegung und Kollisions-Erkennung
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#include <stdlib.h>   /* rand(), abs(), srand(), ... */
#include <stdbool.h>  /* bool‑Typ und true/false Konstanten */
#include <math.h>     /* fabsf(), ceilf(), fmaxf(), ... */
#include <ncurses.h>  /* Terminalgrafik, Tastatur und Farben */
#include <time.h>     /* nanosleep(), struct timespec */
#include "physics.h"  /* Datentypen & Prototypen dieses Moduls */
#include "config.h"   /* Gemeinsame Spielkonstanten */

/* Flag für Flash-Effekt */
int player_flash = 0;  /* Countdown für kurzes Aufblinken des Spieler‑Paddles */
int bot_flash    = 0;  /* Countdown für kurzes Aufblinken des Bot‑Paddles    */

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
    game.player.vx = game.player.ax = 0.0f;


    /* Bot-Paddle initialisieren */
    game.bot          = game.player;
    game.bot.y        = 1;
    game.bot.vx    = game.bot.ax    = 0.0f;

    /* Ball initialisieren */
    game.ball.x  = width  / 2.0f;
    game.ball.y  = height / 2.0f;
    /* Ball horizontal zufällige Richtung */
    game.ball.vx = (rand() & 1) ?  BALL_INITIAL_SPEED : -BALL_INITIAL_SPEED;

    game.ball.vy = -BALL_INITIAL_SPEED;

    game.paddle_hits = 0;
    
    return game;
}

/* Spieler-Paddle bewegen */
void physics_player_update(game_state_t *g, int input_dx)
{
    update_paddle(&g->player,
                  (float)input_dx,               /* -1 / 0 / +1        */
                  PLAYER_ACCELERATION,
                  PLAYER_MAX_SPEED,
                  g->field_width);
}

/* Ball an Paddle-Oberfläche reflektieren und nach jedem Treffer etwas beschleunigen */
/* ---------------------------------------------------------------
 *  Neue Reflexions-Routine mit Mindesttempo + Mindest-Steigung + progressives Tempo
 * -------------------------------------------------------------- */
static void reflect_paddle(ball_t *ball,
                           const paddle_t *p,
                           int hits_since_reset)
{
    /* 1. aktuelles Gesamt-Tempo                                   */
    float speed = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);

    /* 2. Offset (-1 … +1)                                          */
    float mid     = p->x + p->width / 2.0f;
    float offset  = (ball->x - mid) / (p->width / 2.0f);
    if (offset < -1.f) offset = -1.f;
    if (offset >  1.f) offset =  1.f;

    /* 3. Basis-Richtung vor Spin                                   */
    float new_vx  = speed * offset;
    float new_vy  = -copysignf(speed, ball->vy) * (1.f - fabsf(offset));

    /* 4. 30 % Paddle-vx als Spin                                   */
    new_vx += p->vx * 0.30f;

    /* 5. **Dynamischer** Bounce-Faktor                             */
    float bounce = BALL_BOUNCE_MULTIPLIER +
                   hits_since_reset * BALL_BOUNCE_INC;
    ball->vx = new_vx * bounce;
    ball->vy = new_vy * bounce;

    /* 6. Geschwindigkeits-Grenzen                                  */
    float mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);

    /* max                                                           */
    if (mag > BALL_MAX_SPEED) {
        float s = BALL_MAX_SPEED / mag;
        ball->vx *= s; ball->vy *= s; mag = BALL_MAX_SPEED;
    }

    /* **dynamisches Minimum**                                       */
    float dyn_min = BALL_MIN_SPEED *
                    (1.f + hits_since_reset * BALL_MIN_SPEED_INC);
    if (dyn_min > BALL_MAX_SPEED) dyn_min = BALL_MAX_SPEED;

    if (mag < dyn_min) {
        float s = dyn_min / mag;
        ball->vx *= s; ball->vy *= s; mag = dyn_min;
    }

    /* Mindest-Steigung                                              */
    if (fabsf(ball->vy) < mag * BALL_MIN_VY_FRAC) {
        float sign = copysignf(1.f, ball->vy);
        float vy_t = mag * BALL_MIN_VY_FRAC;
        float vx_t = sqrtf(fmaxf(mag*mag - vy_t*vy_t, 0.f));
        ball->vy = sign * vy_t;
        ball->vx = copysignf(vx_t, ball->vx);
    }
}

/* update_paddle()  –  integrierte Beschleunigungsphysik für ein Paddle */
void update_paddle(paddle_t *p,
                   float   dir,         /* -1, 0, +1            */
                   float   accel,       /* gewünschte a         */
                   float   v_max,       /* Maximalgeschw.       */
                   int     field_w)     /* Spielfeldbreite      */
{
    /* Schritt 1 : Beschleunigung bzw. Dämpfung bestimmen */
    if (dir != 0.0f) {
        /* aktiver Input → normale Beschleunigung in Input-Richtung */
        p->ax = accel * dir;
        p->vx += p->ax;
    } else {
        /* kein Input → Geschwindigkeit allmählich abbauen         */
        p->ax = 0.0f;
        p->vx *= PADDLE_DAMPING;
        if (fabsf(p->vx) < PADDLE_STOP_EPS)
            p->vx = 0.0f;
    }

    /* Schritt 2 : Geschwindigkeits‑Deckel */
    if (p->vx >  v_max) p->vx =  v_max;
    if (p->vx < -v_max) p->vx = -v_max;

    /* Schritt 3 : Position aktualisieren */
    p->x += p->vx;

    /* Schritt 4 : Spielfeldgrenzen + „Gummiband“-Effekt */
    float max_x = (float)field_w - p->width - 1.0f;

    /* linke Grenze */
    if (p->x < 0.0f) {
        p->x  = 0.0f;
        p->vx = 0.0f;
    }

    /* rechte Grenze: sobald wir innerhalb einer Zelle vor dem Rand sind
       und weiter nach rechts wollen, sofort ans Limit springen */
    if (dir > 0.0f && p->x >= max_x - 1.0f) {
        p->x  = max_x;
        p->vx = 0.0f;
    }
    /* falls doch einmal minimal übergeschoßen, clamp als Fallback */
    else if (p->x > max_x) {
        p->x  = max_x;
        p->vx = 0.0f;
    }
}

static void reset_ball(game_state_t *game, int dir_down)
{
    /* 1. Ball zentrieren */
    game->ball.x = game->field_width  / 2.0f;
    game->ball.y = game->bot.y + 1;              /* direkt unter Bot  */

    /* 2. Basisgeschwindigkeit abhängig vom Score                */
    float base_speed = BALL_INITIAL_SPEED *
                       (1.0f + game->score * SPEED_PER_POINT);

    /* Optional: Obergrenze, damit es nicht unspielbar wird       */
    if (base_speed > BALL_MAX_SPEED) base_speed = BALL_MAX_SPEED;

    /* 3. Zufällige horizontale Richtung                          */
    game->ball.vx = (rand() & 1) ?  base_speed : -base_speed;

    /* 4. Vertikale Richtung: nach unten (=+), sonst nach oben    */
    game->ball.vy = dir_down ?  base_speed : -base_speed;

    /* 5. Bounce Zähler zurücksetzen */
    game->paddle_hits = 0;
}

/* Zeigt einen kurzen 3‑2‑1‑Countdown in der Bildschirmmitte (je 400 ms) */
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

/* physics_update_ball() – bewegt den Ball schrittweise und prüft
 *                        nach jedem Mini‑Schritt auf Kollisionen.
 *                        false  → Ball ist unten herausgefallen */
bool physics_update_ball(game_state_t *game)
{
    ball_t *ball = &game->ball;

    /*
     * Schritt 1: Anzahl der Mini‑Schritte bestimmen  
     * Wir teilen die Bewegung so fein auf, dass weder dx noch dy
     * größer als eine Terminalzelle ist.
     */
    int sub_steps = (int)ceilf(
        fmaxf(fabsf(ball->vx), fabsf(ball->vy))
    );
    if (sub_steps < 1) sub_steps = 1;   /* Sicherheitsnetz */

    /* Schrittweite pro Sub-Step */
    float step_x = ball->vx / sub_steps;
    float step_y = ball->vy / sub_steps;

    /*
     * Schritt 2: Die berechneten Sub‑Steps nacheinander ausführen
     */
    for (int s = 0; s < sub_steps; ++s)
    {
        ball->x += step_x;
        ball->y += step_y;

        /* Seitenwände (links/rechts) */
        if (ball->x <= 0 || ball->x >= game->field_width - 1)
        {
            /* Ball horizontal umdrehen               */
            ball->vx = -ball->vx;
            step_x   = -step_x;                       /* Rest korrigieren */
            /* Ball in Spielfeld halten               */
            ball->x = fminf(fmaxf(ball->x, 0),
                           game->field_width - 1);
        }

        /* Bot-Paddle (oben) */
        if (ball->vy < 0 &&
            ball->y <= game->bot.y + 1 &&
            ball->y >= game->bot.y &&
            ball->x >= game->bot.x &&
            ball->x <= game->bot.x + game->bot.width)
        {
            game->paddle_hits++;
            reflect_paddle(ball, &game->bot,
                   game->paddle_hits);

            bot_flash = 4;

            step_y = -fabsf(ball->vy) / (sub_steps - s);
        }

        /* Spieler-Paddle (unten) */
        if (ball->vy > 0 &&
            ball->y >= game->player.y - 1 &&
            ball->y <= game->player.y &&
            ball->x >= game->player.x &&
            ball->x <= game->player.x + game->player.width)
        {
            game->paddle_hits++;
            reflect_paddle(ball, &game->player,
                   game->paddle_hits);

            player_flash = 4;

            step_y = -fabsf(ball->vy) / (sub_steps - s);
        }

        /* Punkte & Spielende */
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


