#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void input_process_events(void);
bool input_space_pressed(void);
bool input_space_triggered(void);
bool input_left_triggered(void);
bool input_right_triggered(void);
bool input_pause_triggered(void);
bool input_quit_requested(void);

#endif
