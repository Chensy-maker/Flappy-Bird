#include "render.h"
#include "game.h"
#include <ncurses.h>
#include <string.h>

// 初始化ncurses终端图形库
void render_init(void) {
    initscr();              // 初始化屏幕
    cbreak();               // 禁止行缓冲
    noecho();               // 输入不显示
    curs_set(FALSE);        // 隐藏光标
    nodelay(stdscr, TRUE);  // 非阻塞输入
    keypad(stdscr, TRUE);   // 启用键盘映射
    
    // 启用颜色
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   // 管道和地面
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // 小鸟
        init_pair(3, COLOR_CYAN, COLOR_BLACK);    // 背景元素
        init_pair(4, COLOR_WHITE, COLOR_BLACK);   // UI文字
    }
    
    clear();
    refresh();
}

// 结束渲染，关闭ncurses
void render_quit(void) {
    endwin();
}

// 每一帧刷新画面 - 双缓冲核心：所有绘制完成后只调用一次refresh()
void render_draw(void) {
    // 第1步：清空后台缓冲区（不刷新屏幕）
    erase();
    
    // 第2步：在后台缓冲区绘制所有内容
    draw_bg();
    draw_ground();
    draw_pipes();
    draw_bird();
    draw_ui();
    
    // 第3步：一次性将后台缓冲区刷新到屏幕（双缓冲的关键）
    refresh();
}

// 绘制背景
void draw_bg(void) {
    // 可以在这里添加背景装饰，比如云朵
    attron(COLOR_PAIR(3));
    
    // 简单的云朵装饰
    if (g_scene == SCENE_GAME || g_scene == SCENE_READY) {
        mvaddstr(3, SCREEN_W - 15, "  ___  ");
        mvaddstr(4, SCREEN_W - 15, " (   ) ");
        
        mvaddstr(7, 5, "  ___  ");
        mvaddstr(8, 5, " (   ) ");
    }
    
    attroff(COLOR_PAIR(3));
}

// 绘制地面
void draw_ground(void) {
    attron(COLOR_PAIR(1) | A_BOLD);
    int ground_y = SCREEN_H - 2;
    
    // 绘制地面线
    for (int x = 0; x < SCREEN_W; x++) {
        mvaddch(ground_y, x, '=');
    }
    
    attroff(COLOR_PAIR(1) | A_BOLD);
}

// 绘制小鸟
void draw_bird(void) {
    attron(COLOR_PAIR(2) | A_BOLD);
    int bird_y = (int)g_bird.y;
    
    // 边界检查
    if (bird_y < 0) bird_y = 0;
    if (bird_y >= SCREEN_H - 2) bird_y = SCREEN_H - 3;
    
    // 小鸟动画：两帧翅膀效果
    if (g_bird.frame == 0) {
        mvaddstr(bird_y, BIRD_X, "<o>");
    } else {
        mvaddstr(bird_y, BIRD_X, "<O>");
    }
    
    attroff(COLOR_PAIR(2) | A_BOLD);
}

// 绘制管道
void draw_pipes(void) {
    attron(COLOR_PAIR(1) | A_BOLD);
    
    Pipe *p = g_pipes;
    while (p != NULL) {
        // 只绘制在屏幕内的管道
        if (p->x + PIPE_WIDTH < 0 || p->x >= SCREEN_W) {
            p = p->next;
            continue;
        }
        
        // 上方管道
        for (int y = 0; y < p->gap_y && y < SCREEN_H - 2; y++) {
            for (int w = 0; w < PIPE_WIDTH; w++) {
                int draw_x = p->x + w;
                if (draw_x >= 0 && draw_x < SCREEN_W) {
                    mvaddch(y, draw_x, '|');
                }
            }
        }
        
        // 下方管道
        for (int y = p->gap_y + PIPE_GAP; y < SCREEN_H - 2; y++) {
            for (int w = 0; w < PIPE_WIDTH; w++) {
                int draw_x = p->x + w;
                if (draw_x >= 0 && draw_x < SCREEN_W) {
                    mvaddch(y, draw_x, '|');
                }
            }
        }
        
        p = p->next;
    }
    
    attroff(COLOR_PAIR(1) | A_BOLD);
}

// 绘制UI（分数、标题、提示文字）
void draw_ui(void) {
    attron(COLOR_PAIR(4) | A_BOLD);
    
    switch (g_scene) {
        case SCENE_START:
            mvprintw(SCREEN_H / 2 - 3, SCREEN_W / 2 - 10, "===================");
            mvprintw(SCREEN_H / 2 - 2, SCREEN_W / 2 - 10, "   FLAPPY BIRD    ");
            mvprintw(SCREEN_H / 2 - 1, SCREEN_W / 2 - 10, "===================");
            mvprintw(SCREEN_H / 2 + 1, SCREEN_W / 2 - 15, "Press SPACE to Start");
            mvprintw(SCREEN_H / 2 + 3, SCREEN_W / 2 - 12, "High Score: %d", g_highscore);
            mvprintw(SCREEN_H - 4, SCREEN_W / 2 - 8, "Press Q to Quit");
            break;
            
        case SCENE_READY:
            mvprintw(2, SCREEN_W / 2 - 6, "Get Ready!");
            mvprintw(4, SCREEN_W / 2 - 12, "Press SPACE to Jump");
            mvprintw(SCREEN_H - 4, 2, "Score: %d", g_score);
            break;
            
        case SCENE_GAME:
            mvprintw(1, 2, "Score: %d", g_score);
            mvprintw(1, SCREEN_W - 15, "High: %d", g_highscore);
            break;
            
        case SCENE_OVER:
            mvprintw(SCREEN_H / 2 - 4, SCREEN_W / 2 - 10, "====================");
            mvprintw(SCREEN_H / 2 - 3, SCREEN_W / 2 - 10, "    GAME OVER!     ");
            mvprintw(SCREEN_H / 2 - 2, SCREEN_W / 2 - 10, "====================");
            mvprintw(SCREEN_H / 2, SCREEN_W / 2 - 8, "Your Score: %d", g_score);
            mvprintw(SCREEN_H / 2 + 1, SCREEN_W / 2 - 8, "Best Score: %d", g_highscore);
            mvprintw(SCREEN_H / 2 + 3, SCREEN_W / 2 - 15, "Press SPACE to Restart");
            mvprintw(SCREEN_H / 2 + 5, SCREEN_W / 2 - 8, "Press Q to Quit");
            break;
    }
    
    attroff(COLOR_PAIR(4) | A_BOLD);
}
