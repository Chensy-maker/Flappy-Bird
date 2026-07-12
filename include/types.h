#ifndef TYPES_H
#define TYPES_H

// 终端屏幕尺寸
#define SCREEN_W 60
#define SCREEN_H 25

// 小鸟物理参数
#define BIRD_X 10
#define BIRD_SIZE 2
#define GRAVITY 0.12f
#define JUMP_FORCE -2.2f

// 管道参数
#define PIPE_WIDTH 4
#define PIPE_GAP 6
#define PIPE_SPEED 1
#define PIPE_SPAWN_INTERVAL 25

// 游戏四大场景 有限状态机
typedef enum {
    SCENE_START,
    SCENE_READY,
    SCENE_GAME,
    SCENE_OVER
} GameScene;

// 小鸟结构体
typedef struct {
    float y;
    float vy;
    int frame;  // 翅膀动画帧 0/1
} Bird;

// 管道结构体 链表节点
typedef struct Pipe {
    int x;
    int gap_y;
    struct Pipe *next;
} Pipe, *PipeList;

// AABB碰撞矩形
typedef struct {
    int x, y, w, h;
} Rect;

#endif
