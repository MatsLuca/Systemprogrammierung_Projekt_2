/* ------------------------------------------------------------------
 * main.c - Hauptprogramm und Spielschleife für Console Pong
 * Copyright 2025 Hochschule Hannover
 * Autor: Mats-Luca Dagott, Aseer Al-Hommary
 * 
 * ------------------------------------------------------------------ */

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <locale.h>

#include "input.h"   /* Modul für Tastatureingaben des Spielers */
#include "ai.h"      /* Einfache KI zur Steuerung des Bot‑Schlägers */
#include "physics.h" /* Kollisionsabfragen und Bewegungen von Ball und Schlägern */
#include "render.h"  /* Zeichnet das Spielfeld und die Statusanzeige */
#include "cleanup.h" /* Beendet ncurses sicher und räumt Ressourcen auf */
#include "config.h"  /* Globale Spielkonstanten  */

#define PHYSICS_DT_MS 100   /* Alle 100 ms berechnen wir die Ball‑Physik (≈ 10 FPS) */
#define RENDER_DT_MS   16   /* Alle 16 ms zeichnen wir einen neuen Frame (≈ 60 FPS) */

/* Pausiert das Programm für die angegebene Anzahl von Millisekunden */
static void sleep_ms(unsigned int ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/* Liefert die seit Programmstart vergangene Zeit in Millisekunden */
static unsigned long ms_now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000UL + ts.tv_nsec / 1000000UL);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");   /* Aktiviert Unicode‑Ausgabe im Terminal */
    (void)argc;
    (void)argv;

    srand((unsigned)time(NULL));    /* Initialisiert den Zufallszahl‑Generator */

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    /* Farben, falls Terminal das kann */
    if (has_colors()) {
        start_color();
        use_default_colors();          /* -1 übernimmt jeweils die Hintergrundfarbe des Terminals */
        init_pair(1, COLOR_WHITE,  -1);   /* Farbpaar 1: Weiß auf Standardhintergrund – allgemeiner Text */
        init_pair(2, COLOR_CYAN,   -1);   /* Farbpaar 2: Cyan – Ball */
        init_pair(3, COLOR_YELLOW, -1);   /* Farbpaar 3: Gelb – Spieler‑Schläger */
        init_pair(4, COLOR_MAGENTA,-1);   /* Farbpaar 4: Magenta – KI‑Schläger */
        init_pair(5, COLOR_GREEN,  -1);   /* Farbpaar 5: Grün – Punktestand‑Zeile */
        init_pair(6, COLOR_MAGENTA, -1);   /* Farbpaar 6: Magenta – KI‑Geschwindigkeit */
        init_pair(7, COLOR_CYAN,    -1);   /* Farbpaar 7: Cyan – Ball‑Geschwindigkeit */
    }

    /* Prüft, ob das Terminal groß genug ist */
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    if (max_x < MIN_TERMINAL_WIDTH || max_y < MIN_TERMINAL_HEIGHT)
    {
        endwin();
        printf("Terminal too small! Need at least %dx%d\n", 
               MIN_TERMINAL_WIDTH, MIN_TERMINAL_HEIGHT);
        return EXIT_FAILURE;
    }

    /* Spielsysteme initialisieren */
    input_init();
    render_init();

    game_state_t game = physics_create_game(max_x, max_y);   // Erstellt und initialisiert den kompletten Spielzustand
    unsigned long last_phys = ms_now();                       // Zeitstempel des letzten Physik‑Updates

    /* Haupt-Spielschleife */
    bool running = true;
    while (running)
    {
        /* Eingabe verarbeiten */
        input_action_t action = input_poll();                  // Liest aktuelle Tastatureingaben
        if (action.quit) {
            break;
        }
        physics_player_update(&game, action.dx);               // Bewegt das Spieler‑Paddle entsprechend der Eingabe

        /* Physik und KI werden nur etwa 10‑mal pro Sekunde aktualisiert */
        unsigned long now = ms_now();
        if (now - last_phys >= PHYSICS_DT_MS) {
            ai_update(&game);                                     // Berechnet die neue Position des Bot‑Paddles
            if (!physics_update_ball(&game)) {                     // Aktualisiert Ballposition; false ⇒ Ball verfehlt -> Schleife beenden
                break;
            }
            last_phys = now;
        }

        /* Zeichnet das aktuelle Spielfeld (ca. 60 Frames pro Sekunde) */
        render_frame(&game);
        sleep_ms(RENDER_DT_MS);
    }

    /* Spielende: wartet auf eine Taste, bevor das Programm beendet */
    nodelay(stdscr, FALSE);
    mvprintw(game.field_height / 2, 2, "Game over - press any key");
    refresh();
    getch();

    cleanup_ncurses();      // ncurses‑Modus verlassen und Terminalzustand wiederherstellen

    return EXIT_SUCCESS;
}
