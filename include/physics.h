#ifndef PHYSICS_H
#define PHYSICS_H

#include "types.h"

void bird_apply_gravity(Bird *b);
void bird_jump(Bird *b);
int rect_collide(Rect a, Rect b);
int check_collision(void);
void spawn_pipe(void);
void move_pipes(void);
int check_score(void);

#endif
