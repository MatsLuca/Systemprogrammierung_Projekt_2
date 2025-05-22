#include <ncurses.h>

int main(void)
{
    // Starte ncurses: Terminal für Zeichenausgabe vorbereiten und Bildschirm löschen
    initscr();
    // Verstecke den Cursor für bessere Übersicht
    curs_set(0);
    // Deaktiviere die Anzeige der Tastatureingaben
    noecho();
    // Setze getch() in nicht-blockierenden Modus mit 16 ms Verzögerung (~60 FPS)
    timeout(16);
    // Zeichne einen Rahmen um das Spielfeld
    box(stdscr, 0, 0);
    // Gib eine Startnachricht in Zeile 1, Spalte 2 aus
    mvprintw(1, 2, "Pong starting! Press any key to exit.");
    // Aktualisiere den Bildschirm, damit alle Zeichnungen sichtbar werden
    refresh();
    // Stelle getch() auf blockierenden Modus, um auf einen Tastendruck zu warten
    timeout(-1);
    // Warte auf einen Tastendruck
    getch();
    // Beende ncurses und stelle das Terminal wieder her
    endwin();
    return 0;
}