#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "include/layout.h"

/* initialize ncurses lib */
void init_windows()
{
	/* init ncurses */
	initscr();
	noecho();

	getmaxyx(stdscr, y_max, x_max);

	/* score window */
	score_window_size_y = y_max / 3;
	score_window_size_x = x_max / 3;
	score_window = newwin(score_window_size_y, score_window_size_x, 0, x_max/3*2);

	box(score_window, 0, 0);
	mvwprintw(score_window, 0, score_window_size_x/2-5, "Score");
	wrefresh(score_window);

	score_content = newwin(score_window_size_y-4, score_window_size_x-4, 1, x_max/3*2+1);
	wrefresh(score_content);

	/* client window */
	client_window_size_y = y_max / 3 * 2;
	client_window_size_x = x_max / 3;
	client_window = newwin(client_window_size_y, client_window_size_x, y_max/3, x_max/3*2);

	box(client_window, 0, 0);
	mvwprintw(client_window, 0, client_window_size_x/2-6, "Client");
	wrefresh(client_window);

	client_content = newwin(client_window_size_y-4, client_window_size_x-4, y_max/3+1, x_max/3*2+1);
	wrefresh(client_content);

	/* msg window */
	msg_window_size_y = y_max;
	msg_window_size_x = x_max * 2 / 3;
	msg_window = newwin(msg_window_size_y, msg_window_size_x, 0,0);

	box(msg_window, 0, 0);
	mvwprintw(msg_window, 0, msg_window_size_x/2-7, "Message");
	wrefresh(msg_window);

	msg_content = newwin(msg_window_size_y-4, msg_window_size_x-4, 1, 1);
	scrollok(msg_content, TRUE);
	wrefresh(msg_content);
}
