#ifndef PHYSICS_H
#define PHYSICS_H
#include "types.h"

// 小鸟重力下落
void bird_apply_gravity(Bird *b);
// 小鸟跳跃施加向上力
void bird_jump(Bird *b);
// AABB矩形碰撞
int rect_collide(Rect a, Rect b);
// 小鸟与管道/地面全局碰撞检测
int check_collision(void);
// 生成新管道加入链表
void spawn_pipe(void);
// 管道整体左移
void move_pipes(void);
// 判定是否穿过管道加分
int check_score(void);
#endif
