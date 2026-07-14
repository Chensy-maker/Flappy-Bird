#ifndef TYPES_H
#define TYPES_H

// ============ 屏幕配置 (像素单位) ============
#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60

// ============ 小鸟配置 ============
#define BIRD_X 150
#define BIRD_W 34
#define BIRD_H 24
#define BIRD_JUMP_VEL -8.0f

// ============ 管道配置 ============
#define PIPE_WIDTH 52
#define PIPE_GAP 150
#define PIPE_SPEED 2.0f
#define PIPE_SPAWN_INTERVAL 90
#define PIPE_MIN_HEIGHT 100
#define PIPE_MAX_HEIGHT 400

// ============ 物理配置 ============
#define GRAVITY 0.5f
#define MAX_FALL_SPEED 10.0f

// ============ 矩形结构 ============
typedef struct {
int x, y;
int w, h;
} Rect;

// ============ 游戏场景 ============
typedef enum {
SCENE_START,  // 开始界面
SCENE_READY,  // 准备界面
SCENE_GAME,   // 游戏进行中
SCENE_OVER    // 游戏结束
} GameScene;

// ============ 小鸟结构 ============
typedef struct {
float y;      // Y坐标(像素)
float vy;     // Y方向速度
int frame;    // 动画帧(0-2)
} Bird;

// ============ 管道结构 ============
typedef struct Pipe {
float x;           // X坐标(像素)
int gap_y;         // 间隙中心Y坐标
int scored;        // 是否已计分
struct Pipe *next; // 链表下一个
} Pipe;

typedef Pipe* PipeList;

#endif

