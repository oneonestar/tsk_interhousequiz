#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <ncurses.h>

int x_max, y_max;
int score_window_size_x, score_window_size_y;
int client_window_size_x, client_window_size_y;
int msg_window_size_x, msg_window_size_y;
WINDOW *score_window, *score_content, *client_window, *client_content, *msg_window, *msg_content;

void init_windows();

#endif
