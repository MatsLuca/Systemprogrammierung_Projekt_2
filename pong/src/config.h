/* ------------------------------------------------------------------
 * config.h - Zentrale Spiel-Parameter zum Balancing
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#ifndef CONFIG_H
#define CONFIG_H

// ----- Paddle-Geschwindigkeiten -----
/* -------------  NEUE, frame-basierte Beschleunigungs-Physik ------------- */
#define PLAYER_ACCELERATION        0.70f   /* Zellen / Physik-Frame²   */
#define PLAYER_MAX_SPEED           12.0f   /* Zellen / Physik-Frame    */

#define BOT_BASE_ACCELERATION      0.20f
#define BOT_ACCEL_PER_POINT        0.04f   /* +a pro Score-Punkt       */
#define BOT_MAX_SPEED              10.0f

/* --------- Allgemeine Paddle-Physik --------- */
#define PADDLE_DAMPING          0.80f   /* 0.0 … 1.0  – wie stark vx pro Physik-Frame erhalten bleibt */
#define PADDLE_STOP_EPS         0.05f   /* Schwelle, unter der vx sofort auf 0 gesetzt wird          */

// ----- Ball-Geschwindigkeiten -----
#define BALL_INITIAL_SPEED         0.75f
#define BALL_BOUNCE_MULTIPLIER     1.03f       /* Basis-Bounce-Factor     */
#define SPEED_PER_POINT            0.15f
#define BALL_MAX_SPEED             5.0f

/* Mindesttempo + Steigungs-Limit                                      */
#define BALL_MIN_SPEED             1.0f        /* Baseline-Tempo          */
#define BALL_MIN_VY_FRAC           0.25f       /* ≥ 25 % Vertikalanteil   */

/* *** NEU: Skalierungsfaktoren je Paddle-Hit ************************* */
#define BALL_MIN_SPEED_INC         0.05f       /* +5 %  pro Hit           */
#define BALL_BOUNCE_INC            0.002f      /* +0.2 % pro Hit          */

#endif /* CONFIG_H */
