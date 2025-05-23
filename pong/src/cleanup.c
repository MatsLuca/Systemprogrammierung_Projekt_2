// Cleanup ncurses mode
#include <ncurses.h>
#include "cleanup.h"      /* <- hinzufügen */


void cleanup_ncurses(void)
{
    endwin();
}
