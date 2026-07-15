#ifndef TYPES_H
#define TYPES_H

//==============窗口基础全局配置===================
#define SCREEN_W       800
#define SCREEN_H       600
#define FPS            60//游戏运行帧率

#define GROUND_HEIGHT  112//地面贴图高度
#define GAME_AREA_H    (SCREEN_H - GROUND_HEIGHT)//游戏区域高度
//==============小鸟相关全局配置===================
#define BIRD_X         150//小鸟X坐标（固定不动，小鸟全程只上下移动）
#define BIRD_W         34//小鸟宽度
#define BIRD_H         24//小鸟高度
#define BIRD_JUMP_VEL  -9.0f//小鸟跳跃初速度
#define BIRD_BODY_Y    300//小鸟初始身体Y坐标
//==============管道相关全局配置===================
#define PIPE_WIDTH      52//管道宽度
#define PIPE_GAP        150//管道间隙
#define PIPE_SPEED      2.0f//管道每一帧的移动速度
#define PIPE_SPAWN_INTERVAL  90//管道生成间隔帧数
#define PIPE_MIN_HEIGHT 100//管道最小高度
#define PIPE_HEAD_H     26//管道头部高度
//==============其他全局配置===================
#define GRAVITY         0.5f//重力加速度
#define MAX_FALL_SPEED  10.0f//小鸟最大下落速度

#define BG_SCROLL_SPEED 0.5f//背景滚动速度
#define GROUND_SCROLL_SPEED 2.0f//地面滚动速度

//==============暂停按钮相关配置===================
#define PAUSE_BTN_X     10//暂停按钮左上角x坐标
#define PAUSE_BTN_Y     10//暂停按钮左上角y坐标
#define PAUSE_BTN_W     40//暂停按钮宽度
#define PAUSE_BTN_H     40//暂停按钮高度
#define RESUME_BTN_W    80//继续按钮宽度
#define RESUME_BTN_H    40//继续按钮高度
#define PAUSE_SCROLL_FACTOR 0.3f//暂停时的滚动因子，放慢画面滚动速度

//==============游戏场景枚举===================
typedef enum {
    SCENE_START,
    SCENE_READY,
    SCENE_GAME,
    SCENE_OVER
} GameScene;
//==============小鸟结构体===================
typedef struct {
    float y;
    float vy;//竖直方向速度
    int frame;//翅膀动画帧索引
} Bird;

//==============管道结构体===================
typedef struct Pipe {
    float x;//一组管道整体的横向坐标
    int gap_center;//管道间隙中心位置
    int scored;//得分标记，0表示未得分，1表示已得分
    struct Pipe *next;//单向链表指针，指向下一组管道节点，实现动态无限生成管道
} Pipe;
typedef Pipe *PipeList;//设置别名，方便表示管理管道链表类型，PipeList 等价于 Pipe*
//==============矩形结构体===================
// 用于碰撞检测和绘制等操作
//用于AABB矩形碰撞检测、按钮点击区域判定、贴图绘制范围
typedef struct {
    float x, y, w, h;
} Rect;

#endif

