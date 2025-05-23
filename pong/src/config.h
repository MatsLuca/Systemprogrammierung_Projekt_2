/* ------------------------------------------------------------------
 * config.h - Zentrale Spiel-Parameter zum Balancing
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * ------------------------------------------------------------------ */

#ifndef CONFIG_H
#define CONFIG_H

// ----- Paddle-Geschwindigkeiten -----
// Schrittweite des Spieler-Paddles pro Frame
#define PLAYER_PADDLE_SPEED        5
// Grundgeschwindigkeit des Bot-Paddles
#define BOT_INITIAL_SPEED          2
// Zusätzliche Bot-Geschwindigkeit pro Punkt (linear)
#define BOT_SCORE_SPEED_INCREMENT  1

// ----- Ball-Geschwindigkeiten -----
// Startgeschwindigkeit des Balls
#define BALL_INITIAL_SPEED         0.8f
// Multiplikator bei jedem Abpraller (z.B. an Paddle oder Wand)
#define BALL_BOUNCE_MULTIPLIER     1.05f
/* Ab wie viel Prozent pro Punkt soll der Ball schneller werden? */
#define SPEED_PER_POINT            0.15f    /* 15 % */
/* Obergrenze Geschwindigkeit*/
#define BALL_MAX_SPEED             5.0f

#endif /* CONFIG_H */
