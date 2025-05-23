// Nonblocking keyboard input using ncurses
#include <ncurses.h>
#include "input.h"

void input_init(void)
{
    /* Nothing additional for now */
}

input_action_t input_poll(void)
{
    input_action_t action = {0, 0};
    int ch = getch();
    switch (ch)
    {
    case KEY_LEFT:
        action.dx = -1;
        break;
    case KEY_RIGHT:
        action.dx = 1;
        break;
    case 'q':
    case 'Q':
        action.quit = 1;
        break;
    default:
        break;
    }
    return action;
}
