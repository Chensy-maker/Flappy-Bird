#include "input.h"
#include <ncurses.h>

void input_init(void)
{
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
}

int input_get_key(void)
{
    int ch = getch();
    if(ch == ERR) return 0;
    return ch;
}

void input_restore(void)
{

    nodelay(stdscr, FALSE);
    echo();
    nocbreak();
    curs_set(1);
}