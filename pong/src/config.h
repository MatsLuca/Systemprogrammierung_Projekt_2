/* ------------------------------------------------------------------
 * config.h - Zentrale Spiel-Parameter zum Balancing
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott
 * ------------------------------------------------------------------ */

#ifndef CONFIG_H
#define CONFIG_H

/* ----- Terminal- / Spielfeld-Parameter --------------------------- */
/* Mindestgröße des Terminals für das Spiel */
#define MIN_TERMINAL_WIDTH   20
#define MIN_TERMINAL_HEIGHT  10

/* Verhältnis zur Berechnung der Paddle-Breite */
#define PADDLE_WIDTH_RATIO    6

/* ----- Paddle-Geschwindigkeiten ----------------------------------- */
/* -------------  NEUE, frame-basierte Beschleunigungs-Physik ------------- */
#define PLAYER_ACCELERATION        0.70f   /* Zellen / Physik-Frame²   */
#define PLAYER_MAX_SPEED           12.0f   /* Zellen / Physik-Frame    */

#define BOT_BASE_ACCELERATION      0.20f
#define BOT_ACCEL_PER_POINT        0.04f   /* +a pro Score-Punkt       */
#define BOT_MAX_SPEED              10.0f

/* --------- Allgemeine Paddle-Physik --------- */
/* 0.0 … 1.0 – Faktor, wie stark vx pro Physik‑Frame erhalten bleibt */
#define PADDLE_DAMPING          0.80f
/* Schwelle, unter der vx sofort auf 0 gesetzt wird */
#define PADDLE_STOP_EPS         0.05f

/* ----- Ball-Geschwindigkeiten ------------------------------------- */
#define BALL_INITIAL_SPEED         0.5f
#define BALL_BOUNCE_MULTIPLIER     1.03f       /* Basis-Bounce-Factor     */
#define SPEED_PER_POINT            0.15f
#define BALL_MAX_SPEED             5.0f

/* Mindesttempo + Steigungs-Limit                                      */
#define BALL_MIN_SPEED             1.0f        /* Baseline-Tempo          */
#define BALL_MIN_VY_FRAC           0.25f       /* ≥ 25 % Vertikalanteil   */

/* *** NEU: Skalierungsfaktoren je Paddle-Hit ************************* */
#define BALL_MIN_SPEED_INC         0.05f       /* +5 %  pro Hit           */
#define BALL_BOUNCE_INC            0.002f      /* +0.2 % pro Hit          */

/* Wie stark Rand­treffer abbremsen? */
#define BALL_EDGE_SLOWDOWN  0.25f   

/* ----- UI / Renderer-Parameter ----------------------------------- */
#define FLASH_FRAMES           4      /* Frames, die Paddles aufblinken */
#define COUNTDOWN_STEPS        3      /* 3-2-1 */
#define COUNTDOWN_DELAY_MS     400    /* pro Zahl in Millisekunden */

#endif /* CONFIG_H */
