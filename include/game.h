#ifndef GAME_H
#define GAME_H
// ============ 屏幕配置 (像素单位) ============
#define SCREEN_W 800
#define SCREEN_H 600
#define FPS 60
// ============ 小鸟配置 ============
#define BIRD_X 150           // 小鸟固定X位置
#define BIRD_W 34            // 小鸟宽度
#define BIRD_H 24            // 小鸟高度
#define BIRD_JUMP_VEL -8.0f  // 跳跃速度
// ============ 管道配置 ============
#define PIPE_WIDTH 52        // 管道宽度
#define PIPE_GAP 150         // 管道间隙
#define PIPE_SPEED 2.0f      // 管道移动速度(像素/帧)
#define PIPE_SPAWN_INTERVAL 90  // 管道生成间隔(帧)
#define PIPE_MIN_HEIGHT 100  // 管道最小高度
#define PIPE_MAX_HEIGHT 400  // 管道最大高度
// ============ 物理配置 ============
#define GRAVITY 0.5f         // 重力加速度
#define MAX_FALL_SPEED 10.0f // 最大下落速度
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
// ============ 全局变量 ============
extern GameScene g_scene;
extern Bird      g_bird;
extern PipeList  g_pipes;
extern int       g_score;
extern int       g_highscore;
// ============ 游戏核心函数 ============
void game_init(void);
void game_update(void);
void game_switch_scene(GameScene s);
void game_free_pipes(void);
// ============ 数据持久化 ============
void load_highscore(void);
void save_highscore(void);
#endif