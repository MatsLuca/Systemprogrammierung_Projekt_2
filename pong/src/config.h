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
// Startgeschwindigkeit des Balls
#define BALL_INITIAL_SPEED         0.75f
// Multiplikator bei jedem Abpraller
#define BALL_BOUNCE_MULTIPLIER     1.03f
/* Ab wie viel Prozent pro Punkt soll der Ball schneller werden? */
#define SPEED_PER_POINT            0.15f    /* 15 % */
/* Obergrenze Geschwindigkeit*/
#define BALL_MAX_SPEED             5.0f

#endif /* CONFIG_H */
