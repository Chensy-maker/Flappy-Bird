#include "game.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL.h>
#include <math.h>

GameScene g_scene;//当前游戏场景
Bird      g_bird;//当前小鸟的状态
PipeList  g_pipes;//当前所有管道的链表头指针
int       g_score;//当前游戏分数
int       g_highscore;//历史最高分
int       g_countdown;//倒计时计数器，游戏开始前的倒计时
int       g_running;//游戏主循环是否继续运行的标记，0=不运行，1=运行
float     g_bg_offset;//当前背景图的水平偏移量
float     g_ground_offset;//当前地面图的水平偏移量
SDL_Renderer *g_renderer = NULL;//全局渲染器指针，供其他模块使用

static int pipe_timer = 0;//管道生成计时器
static int ready_timer = 0;//倒计时计时器
static int anim_counter = 0;//小鸟动画帧切换计时器
static float float_t = 0;//小鸟上下浮动的计时器

void game_init(void)
{
    g_scene = SCENE_START;//初始化游戏场景为开始界面
    g_score = 0;//初始化当前分数为0
    g_pipes = NULL;//初始化管道链表为空
    pipe_timer = 0;//初始化管道生成计时器为0
    ready_timer = 0;//初始化倒计时计时器为0
    anim_counter = 0;//初始化小鸟动画帧切换计时器为0
    float_t = 0;//初始化小鸟上下浮动的计时器为0
    g_bg_offset = 0.0f;//初始化背景图水平偏移量为0
    g_ground_offset = 0.0f;//初始化地面图水平偏移量为0

void save_highscore(void)
{
    // 确保data目录存在
    system("mkdir -p data");//创建data目录，如果已经存在则不会报错
    FILE *fp = fopen("data/highscore.txt", "w");//以写入模式打开文件，如果文件不存在则创建，如果存在则清空内容
    if (fp != NULL)//如果文件成功打开
    {
        fprintf(fp, "%d", g_highscore);//把当前最高分写入文件
        fclose(fp);//关闭文件
    }
}

    g_bird.y = BIRD_BODY_Y;//初始化小鸟的垂直位置为屏幕中间
    g_bird.vy = 0.0f;//初始化小鸟的垂直速度为0
    g_bird.frame = 1;//初始化小鸟的动画帧为1（中间帧）

    load_highscore();//加载历史最高分
}

void game_free_pipes(void)
{
    PipeList p = g_pipes, q;//遍历管道链表，释放每个管道节点的内存
    while (p != NULL) {//当当前管道节点不为空时
        q = p->next;//保存下一个管道节点的指针
        free(p);//释放当前管道节点的内存
        p = q;//移动到下一个管道节点
    }
    g_pipes = NULL;//把管道链表头指针置为空，表示没有管道
}

void game_cleanup_offscreen_pipes(void)
{
    PipeList *curr = &g_pipes;//使用指针的指针来遍历管道链表，方便删除节点
    while (*curr != NULL) {//当当前管道节点不为空时
        if ((*curr)->x + PIPE_WIDTH < -50) {//如果当前管道节点的右边界已经完全离开屏幕左边界（-50像素），则删除该管道节点
            PipeList to_free = *curr;//保存当前管道节点的指针，用于释放内存
            *curr = (*curr)->next;//把当前管道节点从链表中移除，指向下一个管道节点
            free(to_free);//释放当前管道节点的内存
        } else {
            curr = &(*curr)->next;//移动到下一个管道节点
        }
    }
}

void game_switch_scene(GameScene s)
{
    g_scene = s;//切换游戏场景为 s

    if (s == SCENE_READY) {//如果切换到准备场景，重置游戏状态
        g_score = 0;
        pipe_timer = 0;
        ready_timer = 0;
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        game_free_pipes();
    }

    if (s == SCENE_START) {//如果切换到开始场景，重置游戏状态
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        game_free_pipes();
        g_score = 0;
    }
}

void game_update(void)
{
    switch (g_scene) {
    case SCENE_START: {
        g_bg_offset -= BG_SCROLL_SPEED;//背景图水平偏移量每帧减少 BG_SCROLL_SPEED 像素
        g_ground_offset -= GROUND_SCROLL_SPEED;//地面图水平偏移量每帧减少 GROUND_SCROLL_SPEED 像素

        float_t += 0.05f;//更新小鸟上下浮动的计时器
        g_bird.y = BIRD_BODY_Y + (float)sin(float_t) * 5.0f;//根据正弦函数计算小鸟的垂直位置

        anim_counter++;//更新小鸟动画帧切换计时器
        if (anim_counter >= 8) {//每8帧切换一次小鸟动画帧
            anim_counter = 0;//重置计时器
            g_bird.frame = (g_bird.frame + 1) % 3;//切换小鸟动画帧，循环在0、1、2之间
        }

        if (input_space_triggered()) {//如果空格键刚刚按下，切换到准备场景
            audio_play_swoosh();//播放切换场景音效
            game_switch_scene(SCENE_READY);//切换到准备场景
        }
        break;
    }

    case SCENE_READY: {
        g_bg_offset -= BG_SCROLL_SPEED;//背景图水平偏移量每帧减少 BG_SCROLL_SPEED 像素
        g_ground_offset -= GROUND_SCROLL_SPEED;//地面图水平偏移量每帧减少 GROUND_SCROLL_SPEED 像素

        float_t += 0.05f;//更新小鸟上下浮动的计时器
        g_bird.y = BIRD_BODY_Y + (float)sin(float_t) * 5.0f;//根据正弦函数计算小鸟的垂直位置

        anim_counter++;//更新小鸟动画帧切换计时器
        if (anim_counter >= 8) {//每8帧切换一次小鸟动画帧
            anim_counter = 0;//重置计时器
            g_bird.frame = (g_bird.frame + 1) % 3;//切换小鸟动画帧，循环在0、1、2之间
        }

        ready_timer++;//更新倒计时计时器
        if (ready_timer >= FPS * 3) {//如果倒计时计时器达到3秒，切换到游戏场景
            g_countdown = 0;//重置倒计时计数器
            g_scene = SCENE_GAME;//切换到游戏场景
        } else {
            g_countdown = 3 - ready_timer / FPS;//计算倒计时剩余秒数
            if (g_countdown < 1) g_countdown = 1;//确保倒计时不会小于1
        }

        if (input_space_triggered()) {//如果空格键刚刚按下，切换到游戏场景
            audio_play_swoosh();//播放切换场景音效
            g_scene = SCENE_GAME;//切换到游戏场景
        }
        break;
    }

    case SCENE_GAME: {//游戏主循环
        g_bg_offset -= BG_SCROLL_SPEED;//背景图水平偏移量每帧减少 BG_SCROLL_SPEED 像素
        g_ground_offset -= GROUND_SCROLL_SPEED;//地面图水平偏移量每帧减少 GROUND_SCROLL_SPEED 像素

        anim_counter++;//更新小鸟动画帧切换计时器
        if (anim_counter >= 5) {//每5帧切换一次小鸟动画帧
            anim_counter = 0;//重置计时器
            g_bird.frame++;//切换小鸟动画帧
            if (g_bird.frame > 2) g_bird.frame = 0;//如果动画帧超过2，循环回0
        }

        bird_apply_gravity(&g_bird);//应用重力更新小鸟的垂直位置和速度

        if (input_space_triggered()) {//如果空格键刚刚按下，小鸟跳跃
            bird_jump(&g_bird);//调用小鸟跳跃函数，改变小鸟的垂直速度
            audio_play_wing();//播放小鸟扇动翅膀的音效
        }

        pipe_timer++;//更新管道生成计时器
        if (pipe_timer >= PIPE_SPAWN_INTERVAL) {//如果管道生成计时器达到设定的间隔，生成新的管道
            pipe_timer = 0;//重置管道生成计时器
            spawn_pipe();//调用生成管道函数，创建新的管道并加入链表
        }

        move_pipes();//调用移动管道函数，更新所有管道的位置
        game_cleanup_offscreen_pipes();//清理屏幕外的管道，释放内存

        if (check_collision()) {//如果小鸟与管道或地面发生碰撞，切换到游戏结束场景
            audio_play_hit();//播放小鸟碰撞的音效
            audio_play_die();//播放小鸟死亡的音效
            if (g_score > g_highscore) {
                g_highscore = g_score;
                save_highscore();//保存新的历史最高分
            }
            g_scene = SCENE_OVER;//切换到游戏结束场景
            break;
        }

        if (check_score()) {//如果小鸟成功通过一对管道，增加分数
            g_score++;//增加当前分数
            audio_play_point();//播放得分音效
        }
        break;
    }

    case SCENE_OVER: {
        g_bg_offset -= BG_SCROLL_SPEED * 0.5f;//背景图水平偏移量每帧减少 BG_SCROLL_SPEED * 0.5 像素，减慢背景滚动速度
        g_ground_offset -= GROUND_SCROLL_SPEED * 0.5f;//地面图水平偏移量每帧减少 GROUND_SCROLL_SPEED * 0.5 像素，减慢地面滚动速度

        bird_apply_gravity(&g_bird);//应用重力更新小鸟的垂直位置和速度
        if (g_bird.y > SCREEN_H - GROUND_HEIGHT - BIRD_H) {//如果小鸟的垂直位置超过地面高度，限制小鸟的位置在地面上
            g_bird.y = (float)(SCREEN_H - GROUND_HEIGHT - BIRD_H);//限制小鸟的位置在地面上
        }

        if (input_space_triggered()) {//如果空格键刚刚按下，切换到开始场景
            audio_play_swoosh();//播放切换场景音效
            game_switch_scene(SCENE_START);//切换到开始场景
        }
        break;
    }
    }
}

void load_highscore(void)//加载历史最高分
{
    FILE *fp = fopen("data/highscore.txt", "r");//以只读模式打开文件，如果文件不存在则返回 NULL
    g_highscore = 0;//初始化历史最高分为0
    if (fp != NULL) {
        if (fscanf(fp, "%d", &g_highscore) != 1)//从文件中读取一个整数，如果读取失败则保持 g_highscore 为0
            g_highscore = 0;//如果读取失败，保持 g_highscore 为0
        fclose(fp);//关闭文件
    }
}

void save_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "w");//以写入模式打开文件，如果文件不存在则创建，如果存在则清空内容
    if (fp != NULL) {
        fprintf(fp, "%d", g_highscore);//把当前最高分写入文件
        fclose(fp);//关闭文件
    }
}
