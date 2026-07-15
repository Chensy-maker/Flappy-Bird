#ifndef TYPES_H
#define TYPES_H

#define SCREEN_W       800
#define SCREEN_H       600
#define FPS            60

// ============ 小鸟配置 ============
#define BIRD_X 150
#define BIRD_W 34
#define BIRD_H 24
#define BIRD_JUMP_VEL -9.0f   

#define BIRD_X         150
#define BIRD_W         34
#define BIRD_H         24
#define BIRD_JUMP_VEL  -9.0f
#define BIRD_BODY_Y    300

#define PIPE_WIDTH      52
#define PIPE_GAP        150
#define PIPE_SPEED      2.0f
#define PIPE_SPAWN_INTERVAL  90
#define PIPE_MIN_HEIGHT 100
#define PIPE_HEAD_H     26

#define GRAVITY         0.55f
#define MAX_FALL_SPEED  10.0f

#define BG_SCROLL_SPEED 0.5f
#define GROUND_SCROLL_SPEED 2.0f
/* ---------- ��ͣ / ���� ��ť ---------- */
#define PAUSE_BTN_X     10
#define PAUSE_BTN_Y     10
#define PAUSE_BTN_W     40
#define PAUSE_BTN_H     40
#define RESUME_BTN_W    80
#define RESUME_BTN_H    40
#define PAUSE_SCROLL_FACTOR 0.3f


typedef enum {
    SCENE_START,
    SCENE_READY,
    SCENE_GAME,
    SCENE_OVER
} GameScene;

typedef struct {
    float y;
    float vy;
    int frame;
} Bird;

typedef struct Pipe {
    float x;
    int gap_center;
    int scored;
    struct Pipe *next;
} Pipe;
typedef Pipe *PipeList;

typedef struct {
    float x, y, w, h;
} Rect;

#endif

