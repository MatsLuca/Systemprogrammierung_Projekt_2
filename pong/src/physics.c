/* ------------------------------------------------------------------
 * physics.c - Ballbewegung und Kollisions-Erkennung
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#include <stdlib.h>   /* rand(), abs(), srand(), ... */
#include <stdbool.h>  /* bool‑Typ und true/false Konstanten */
#include <math.h>     /* fabsf(), ceilf(), fmaxf(), ... */
#include <time.h>     /* struct timespec */
#include "physics.h"  /* Datentypen & Prototypen dieses Moduls */
#include "config.h"   /* Gemeinsame Spielkonstanten */

/* Keine UI-Zustände/Globals mehr hier – reine Physik */

/* RNG-Provider (injizierbar für Tests/Konfiguration) */
static unsigned int physics_rand_default(void) { return (unsigned int)rand(); }
static unsigned int (*physics_rand)(void) = physics_rand_default;

void physics_seed(unsigned int seed) { srand(seed); }
void physics_set_random_provider(unsigned int (*rand_func)(void))
{
    physics_rand = rand_func ? rand_func : physics_rand_default;
}

/* ------------------------------------------------------------------
 * physics_create_game
 * Erzeugt einen initialisierten Spielzustand mit gültiger Feldgröße.
 *
 * Parameter:
 *   width  – gewünschte Spielfeldbreite
 *   height – gewünschte Spielfeldhöhe
 *
 * Rückgabe:
 *   game_state_t mit allen Anfangswerten
 * ------------------------------------------------------------------ */
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
    game.ball.vx = (physics_rand() & 1u) ?  BALL_INITIAL_SPEED : -BALL_INITIAL_SPEED;

    game.ball.vy = -BALL_INITIAL_SPEED;

    game.paddle_hits = 0;
    
    return game;
}

/* ------------------------------------------------------------------
 * physics_player_update
 * Aktualisiert Position und Geschwindigkeit des Spieler‑Paddles
 * basierend auf der aktuellen Eingabe.
 *
 * Parameter:
 *   g        – Zeiger auf Spielzustand
 *   input_dx – Bewegungsrichtung (-1, 0, +1)
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
void physics_player_update(game_state_t *g, int input_dx)
{
    update_paddle(&g->player,
                  (float)input_dx,               /* -1 / 0 / +1        */
                  PLAYER_ACCELERATION,
                  PLAYER_MAX_SPEED,
                  g->field_width);
}

/* ------------------------------------------------------------------
 * reflect_paddle
 * Berechnet die neue Ballrichtung und -geschwindigkeit nach einem
 * Aufprall auf ein Paddle und wendet Spin, Bounce‑Faktor und Limits an.
 *
 * Parameter:
 *   ball             – Zeiger auf den Ball
 *   p                – getroffener Schläger
 *   hits_since_reset – Anzahl Paddle‑Hits seit letztem Reset
 *
 * Rückgabe:
 *   keine (Ball wird in-place geändert)
 * ------------------------------------------------------------------ */
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

    /* 3. Richtung vor Spin  ----------------------------------------- */
    float abs_off = fabsf(offset);      /* 0 … 1                       */

    /* a)  Vektor mit **konstanter** Länge = speed                     */
    float new_vx = speed * offset;
    float new_vy = -copysignf(
                    speed * sqrtf(1.0f - abs_off * abs_off),
                    ball->vy);

    /* 4. 30 % Paddle-vx als Spin                                   */
    new_vx += p->vx * 0.30f;

    /* 5. **Dynamischer** Bounce-Faktor                             */
    float bounce = BALL_BOUNCE_MULTIPLIER +
                   hits_since_reset * BALL_BOUNCE_INC;

    /* 5. Bounce auf die Richtung anwenden                          */
    new_vx *= bounce;
    new_vy *= bounce;

    /* 6. Edge‑Drop jetzt – wirkt nur, wenn gewünscht               */
    float edge_drop = 1.0f - BALL_EDGE_SLOWDOWN * abs_off;   /* center: 1.0 … edge: 1.0‑slowdown */
    new_vx *= edge_drop;
    new_vy *= edge_drop;

    /* 7. Ergebnis in Ball schreiben                                */
    ball->vx = new_vx;
    ball->vy = new_vy;

    /* 8. Geschwindigkeits-Grenzen                                  */
    float mag = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);

    /* max                                                           */
    if (mag > BALL_MAX_SPEED) {
        float s = BALL_MAX_SPEED / mag;
        ball->vx *= s; ball->vy *= s; mag = BALL_MAX_SPEED;
    }

    /* 9. **dynamisches Minimum**                                    */
    float dyn_min = BALL_MIN_SPEED *
                    (1.f + hits_since_reset * BALL_MIN_SPEED_INC);
    if (dyn_min > BALL_MAX_SPEED) dyn_min = BALL_MAX_SPEED;

    if (mag < dyn_min) {
        float s = dyn_min / mag;
        ball->vx *= s; ball->vy *= s; mag = dyn_min;
    }

    /* 10. Mindest-Steigung                                          */
    if (fabsf(ball->vy) < mag * BALL_MIN_VY_FRAC) {
        float sign = copysignf(1.f, ball->vy);
        float vy_t = mag * BALL_MIN_VY_FRAC;
        float vx_t = sqrtf(fmaxf(mag*mag - vy_t*vy_t, 0.f));
        ball->vy = sign * vy_t;
        ball->vx = copysignf(vx_t, ball->vx);
    }
}

/* ------------------------------------------------------------------
 * update_paddle
 * Integriert Beschleunigung, Dämpfung, Clamping und Position des
 * angegebenen Paddles für einen Physik‑Frame.
 *
 * Parameter:
 *   p       – Zeiger auf Paddle
 *   dir     – gewünschte Richtung (-1, 0, +1)
 *   accel   – Beschleunigung
 *   v_max   – Maximalgeschwindigkeit
 *   field_w – Spielfeldbreite
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
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

/* ------------------------------------------------------------------
 * reset_ball
 * Setzt den Ball nach einem Punkt an die Ausgangsposition und
 * skaliert seine Startgeschwindigkeit anhand des aktuellen Scores.
 *
 * Parameter:
 *   game     – Zeiger auf Spielzustand
 *   dir_down – 1 = Ball startet nach unten, 0 = nach oben
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
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
    game->ball.vx = (physics_rand() & 1u) ?  base_speed : -base_speed;

    /* 4. Vertikale Richtung: nach unten (=+), sonst nach oben    */
    game->ball.vy = dir_down ?  base_speed : -base_speed;

    /* 5. Bounce Zähler zurücksetzen */
    game->paddle_hits = 0;
}

/* ------------------------------------------------------------------
 * show_countdown
 * Zeigt einen kurzen 3‑2‑1‑Countdown in der Bildschirmmitte und
 * pausiert jeweils 400 ms zwischen den Zahlen.
 *
 * Parameter:
 *   keine
 *
 * Rückgabe:
 *   keine
 * ------------------------------------------------------------------ */
/* Countdown wird in der UI realisiert – Physik emittiert nur SCORED-Event */

/* ------------------------------------------------------------------
 * physics_update_ball
 * Bewegt den Ball in kleinen Schritten, prüft Kollisionen mit Wänden
 * und Paddles und behandelt Punkte sowie Spielende.
 *
 * Parameter:
 *   game – Zeiger auf Spielzustand
 *
 * Rückgabe:
 *   true  – Spiel läuft weiter
 *   false – Ball ist unten herausgefallen (Game Over)
 * ------------------------------------------------------------------ */
physics_event_t physics_update_ball_events(game_state_t *game)
{
    ball_t *ball = &game->ball;
    physics_event_t events = PHYS_EVENT_NONE;

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
            events |= PHYS_EVENT_HIT_BOT;

            /* Schrittgrößen ab diesem Sub-Step neu kalibrieren */
            step_x = ball->vx / (sub_steps - s);
            step_y = ball->vy / (sub_steps - s);
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
            events |= PHYS_EVENT_HIT_PLAYER;

            /* Schrittgrößen ab diesem Sub-Step neu kalibrieren */
            step_x = ball->vx / (sub_steps - s);
            step_y = ball->vy / (sub_steps - s);
        }

        /* Punkte & Spielende */
        if (ball->y < 0)                                      /* oben raus -> Punkt  */
        {
            game->score += 1;
            reset_ball(game, /*dir_down=*/1);
            events |= PHYS_EVENT_SCORED;
            break;                                            /* Frame fertig       */
        }
        else if (ball->y > game->field_height)                /* unten raus -> Ende */
        {
            events |= PHYS_EVENT_GAME_OVER;
            return events;
        }
    }

    return events;  /* Events dieses Updates */
}

bool physics_update_ball(game_state_t *game)
{
    physics_event_t ev = physics_update_ball_events(game);
    return (ev & PHYS_EVENT_GAME_OVER) == 0;
}


