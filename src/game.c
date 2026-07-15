#include "game.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

GameScene g_scene;//当前游戏场景
Bird      g_bird;//小鸟结构体实例
PipeList  g_pipes;//管道链表头指针
int       g_score;//当前得分
int       g_highscore;//历史最高分
int       g_countdown;//准备场景倒计时
int       g_running;//游戏主循环运行标志
int       g_paused = 0;//游戏暂停标志
int       g_bird_color = 0;//小鸟颜色索引，0表示红色，1表示蓝色，2表示黄色
int       g_is_night = 0;//是否为夜间模式，1表示夜间，0表示白天
float     g_bg_offset;//背景滚动偏移量
float     g_ground_offset;//地面滚动偏移量
SDL_Renderer *g_renderer = NULL;//SDL渲染器实例

static int pipe_timer = 0;//管道生成计时器
static int ready_timer = 0;//准备场景计时器
static int anim_counter = 0;//小鸟翅膀动画计时器
static float frame_t = 0;//小鸟上下浮动的计时器

void game_init(void)
{
    time_t t = time(NULL);//获取当前时间戳
    struct tm *tm = localtime(&t);//将时间戳转换为本地时间结构体
    int hour = tm->tm_hour;//获取当前小时数（0-23）
    g_is_night = (hour >= 19 || hour < 6) ? 1 : 0;//根据当前小时数判断是否为夜间模式，19点到次日6点为夜间模式

    g_scene = SCENE_START;//初始化游戏场景为开始场景
    g_score = 0;//初始化当前得分为0
    g_pipes = NULL;//初始化管道链表为空
    pipe_timer = 0;//初始化管道生成计时器为0
    ready_timer = 0;//初始化准备场景计时器为0
    anim_counter = 0;//初始化小鸟翅膀动画计时器为0
    frame_t = 0;//初始化小鸟上下浮动的计时器为0
    g_bg_offset = 0.0f;//初始化背景滚动偏移量为0
    g_ground_offset = 0.0f;//初始化地面滚动偏移量为0
    g_bird_color = 0;//初始化小鸟颜色索引为0（红色）
    g_paused = 0;//初始化游戏暂停标志为0（未暂停）

    g_bird.y = BIRD_BODY_Y;//初始化小鸟的垂直位置为初始身体Y坐标
    g_bird.vy = 0.0f;//初始化小鸟的竖直速度为0
    g_bird.frame = 1;//初始化小鸟的翅膀动画帧索引为1（中间帧）

    load_highscore();//加载历史最高分
}

void game_free_pipes(void)//释放管道链表内存
{
    PipeList p = g_pipes, q;//遍历管道链表，释放每个节点的内存
    while (p != NULL) {//当当前节点不为空时，继续遍历
        q = p->next;//保存下一个节点的指针
        free(p);//释放当前节点的内存
        p = q;//移动到下一个节点
    }
    g_pipes = NULL;//将管道链表头指针置为空，表示链表已清空
}

void game_cleanup_offscreen_pipes(void)
{
    PipeList *curr = &g_pipes;//使用指向指针的指针遍历管道链表，方便删除节点
    while (*curr != NULL) {
        if ((*curr)->x + PIPE_WIDTH < -50) {//如果管道已经完全移出屏幕左侧，删除该节点
            PipeList to_free = *curr;//保存当前节点的指针，准备释放内存
            *curr = (*curr)->next;//将当前节点从链表中移除，指向下一个节点
            free(to_free);//释放当前节点的内存
        } else {
            curr = &(*curr)->next;//移动到下一个节点
        }
    }
}

void game_switch_scene(GameScene s)//切换游戏场景
{
    g_scene = s;

    if (s == SCENE_READY) {//切换到准备场景时，重置游戏状态
        g_score = 0;
        g_paused = 0;
        pipe_timer = 0;
        ready_timer = 0;
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        game_free_pipes();
    }

    if (s == SCENE_START) {//切换到开始场景时，重置游戏状态
        g_bird.y = BIRD_BODY_Y;
        g_bird.vy = 0.0f;
        g_bird.frame = 1;
        g_paused = 0;
        game_free_pipes();
        g_score = 0;
    }
}

void game_update(void)
{
    switch (g_scene) {
    case SCENE_START: {
        g_bg_offset -= BG_SCROLL_SPEED;//背景滚动偏移量每帧减少背景滚动速度
        g_ground_offset -= GROUND_SCROLL_SPEED;//地面滚动偏移量每帧减少地面滚动速度

        frame_t += 0.05f;//小鸟上下浮动的计时器每帧增加0.05
        g_bird.y = BIRD_BODY_Y + (float)sin(frame_t) * 5.0f;//小鸟的垂直位置根据正弦函数上下浮动，幅度为5像素

        anim_counter++;//小鸟翅膀动画计时器每帧增加1
        if (anim_counter >= 8) {//当计时器达到8帧时，切换小鸟的翅膀动画帧
            anim_counter = 0;//重置计时器为0
            g_bird.frame = (g_bird.frame + 1) % 3;//小鸟的翅膀动画帧索引在0、1、2之间循环
        }

        if (input_left_triggered()) {//左方向键按下，切换小鸟颜色
            g_bird_color = (g_bird_color + 2) % 3;//小鸟颜色索引在0、1、2之间循环，按下左方向键时颜色索引减1，使用加2再取模3实现循环
            audio_play_swoosh();//播放切换小鸟颜色的音效
        }
        if (input_right_triggered()) {//右方向键按下，切换小鸟颜色
            g_bird_color = (g_bird_color + 1) % 3;//小鸟颜色索引在0、1、2之间循环，按下右方向键时颜色索引加1
            audio_play_swoosh();//播放切换小鸟颜色的音效
        }

        if (input_click_triggered()) {//鼠标左键点击，切换到准备场景
            audio_play_swoosh();//播放切换场景的音效
            game_switch_scene(SCENE_READY);//切换到准备场景
        }
        break;
    }

    case SCENE_READY: {
        g_bg_offset -= BG_SCROLL_SPEED;//背景滚动偏移量每帧减少背景滚动速度
        g_ground_offset -= GROUND_SCROLL_SPEED;//地面滚动偏移量每帧减少地面滚动速度

        frame_t += 0.05f;//小鸟上下浮动的计时器每帧增加0.05
        g_bird.y = BIRD_BODY_Y + (float)sin(frame_t) * 5.0f;//小鸟的垂直位置根据正弦函数上下浮动，幅度为5像素

        anim_counter++;//小鸟翅膀动画计时器每帧增加1
        if (anim_counter >= 8) {//当计时器达到8帧时，切换小鸟的翅膀动画帧
            anim_counter = 0;//重置计时器为0
            g_bird.frame = (g_bird.frame + 1) % 3;//小鸟的翅膀动画帧索引在0、1、2之间循环
        }

        ready_timer++;//准备场景计时器每帧增加1
        if (ready_timer >= FPS * 3) {//当计时器达到3秒时，切换到游戏场景
            g_countdown = 0;//倒计时归零
            g_scene = SCENE_GAME;//切换到游戏场景
        } else {
            g_countdown = 3 - ready_timer / FPS;//计算倒计时，3秒倒计时，每秒减少1
            if (g_countdown < 1) g_countdown = 1;//确保倒计时不会小于1
        }

        if (input_click_triggered()) {//鼠标左键点击，立即切换到游戏场景
            audio_play_swoosh();//播放切换场景的音效
            g_scene = SCENE_GAME;//切换到游戏场景
        }
        break;
    }

    case SCENE_GAME: {
        /* 空格 / P / PAUSE → 切换暂停 */
        if (input_space_triggered() || input_pause_triggered()) {//空格键或暂停键刚刚按下，切换暂停状态
            g_paused = !g_paused;
        }

        if (g_paused) {
            /* 暂停时：背景和地面慢速滚动，其余全部冻结 */
            g_bg_offset -= BG_SCROLL_SPEED * PAUSE_SCROLL_FACTOR;//背景滚动偏移量每帧减少背景滚动速度的暂停因子
            g_ground_offset -= GROUND_SCROLL_SPEED * PAUSE_SCROLL_FACTOR;//地面滚动偏移量每帧减少地面滚动速度的暂停因子
        } else {
            g_bg_offset -= BG_SCROLL_SPEED;//背景滚动偏移量每帧减少背景滚动速度
            g_ground_offset -= GROUND_SCROLL_SPEED;//地面滚动偏移量每帧减少地面滚动速度

            anim_counter++;
            if (anim_counter >= 5) {//小鸟翅膀动画计时器每5帧切换一次
                anim_counter = 0;//重置计时器为0
                g_bird.frame++;//小鸟的翅膀动画帧索引加1
                if (g_bird.frame > 2) g_bird.frame = 0;//小鸟的翅膀动画帧索引在0、1、2之间循环
            }

            bird_apply_gravity(&g_bird);//应用重力加速度，更新小鸟的竖直速度和位置

            pipe_timer++;
            if (pipe_timer >= PIPE_SPAWN_INTERVAL) {//当管道生成计时器达到管道生成间隔帧数时，生成新的一组管道
                pipe_timer = 0;//重置管道生成计时器为0
                spawn_pipe();//生成新的一组管道
            }

            move_pipes();//移动所有管道，更新它们的横向位置
            game_cleanup_offscreen_pipes();//清理已经完全移出屏幕左侧的管道，释放它们的内存

            if (check_collision()) {//检查小鸟是否与管道或地面发生碰撞
                audio_play_hit();//播放碰撞音效
                audio_play_die();//播放死亡音效
                if (g_score > g_highscore) {//如果当前得分大于历史最高分，更新历史最高分并保存到文件
                    g_highscore = g_score;//更新历史最高分
                    save_highscore();//保存历史最高分到文件
                }
                g_scene = SCENE_OVER;//切换到游戏结束场景
                break;
            }

            if (check_score()) {//检查小鸟是否成功通过一组管道，增加得分
                g_score++;//当前得分加1
                audio_play_point();//播放得分音效
            }
        }

        /* 鼠标点击处理 */
        if (input_click_triggered()) {
            if (g_paused) {
                /* 暂停时：点击中间继续按钮区域 → 恢复 */
                int rx = SCREEN_W / 2 - RESUME_BTN_W / 2;//继续按钮左上角x坐标
                int ry = SCREEN_H / 2 - RESUME_BTN_H / 2;//继续按钮左上角y坐标
                if (click_x >= rx && click_x <= rx + RESUME_BTN_W &&//鼠标点击的x坐标在继续按钮区域内
                    click_y >= ry && click_y <= ry + RESUME_BTN_H) {//鼠标点击的y坐标在继续按钮区域内
                    g_paused = 0;//恢复游戏
                }
            } else {
                /* 运行中：点击左上角暂停按钮 → 暂停 */
                if (click_x >= PAUSE_BTN_X && click_x <= PAUSE_BTN_X + PAUSE_BTN_W &&//鼠标点击的x坐标在暂停按钮区域内
                    click_y >= PAUSE_BTN_Y && click_y <= PAUSE_BTN_Y + PAUSE_BTN_H) {//鼠标点击的y坐标在暂停按钮区域内
                    g_paused = 1;
                } else {
                    /* 点击其他区域 → 小鸟跳跃 */
                    bird_jump(&g_bird);//小鸟跳跃，更新竖直速度
                    audio_play_wing();//播放小鸟翅膀扇动音效
                }
            }
        }
        break;
    }

    case SCENE_OVER: {
        g_bg_offset -= BG_SCROLL_SPEED * 0.5f;//背景滚动偏移量每帧减少背景滚动速度的一半
        g_ground_offset -= GROUND_SCROLL_SPEED * 0.5f;//地面滚动偏移量每帧减少地面滚动速度的一半

        bird_apply_gravity(&g_bird);
        if (g_bird.y > SCREEN_H - GROUND_HEIGHT - BIRD_H) {//如果小鸟的垂直位置超过地面高度，修正小鸟位置为地面上方
            g_bird.y = (float)(SCREEN_H - GROUND_HEIGHT - BIRD_H);//修正小鸟位置为地面上方
        }

        if (input_click_triggered()) {//鼠标左键点击，切换到开始场景
            audio_play_swoosh();//播放切换场景的音效
            game_switch_scene(SCENE_START);//切换到开始场景
        }
        break;
    }
    }
}

void load_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "r");//尝试打开存储历史最高分的文件
    g_highscore = 0;
    if (fp != NULL) {//如果文件存在，读取历史最高分
        if (fscanf(fp, "%d", &g_highscore) != 1)//如果读取失败，设置历史最高分为0
            g_highscore = 0;//读取历史最高分
        fclose(fp);//关闭文件
    }
}

void save_highscore(void)
{
    FILE *fp = fopen("data/highscore.txt", "w");//尝试打开存储历史最高分的文件，如果不存在则创建新文件
    if (fp != NULL) {
        fprintf(fp, "%d", g_highscore);//将历史最高分写入文件
        fclose(fp);//关闭文件
    }
}
