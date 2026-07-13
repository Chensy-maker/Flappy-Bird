#include "render.h"
#include "game.h"
#include <ncurses.h>
#include <string.h>
// 初始化ncurses终端图形库和双缓冲
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
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   // 管道
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // 小鸟
        init_pair(3, COLOR_CYAN, COLOR_BLACK);    // 背景
        init_pair(4, COLOR_WHITE, COLOR_BLACK);   // UI文字
    }
    
    clear();
    refresh();
}
// 结束渲染，关闭ncurses
void render_quit(void) {
    endwin();
}
// 每一帧刷新画面
void render_draw(void) {
    erase();
    draw_bg();
    draw_ground();
    draw_pipes();
    draw_bird();
    draw_ui();
    refresh();
}
// 绘制滚动背景
void draw_bg(void) {
    // 空函数 - 让 erase() 处理背景清空
    // 不需要逐字符填充，减少闪烁
}
// 绘制地面
void draw_ground(void) {
    attron(COLOR_PAIR(1) | A_BOLD);
    int ground_y = SCREEN_H - 2;
    for (int x = 0; x < SCREEN_W; x++) {
        mvaddch(ground_y, x, '=');
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
}
// 绘制小鸟
void draw_bird(void) {
    attron(COLOR_PAIR(2) | A_BOLD);
    int bird_y = (int)g_bird.y;
    
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
        // 上方管道
        for (int y = 0; y < p->gap_y; y++) {
            for (int w = 0; w < PIPE_WIDTH; w++) {
                if (p->x + w >= 0 && p->x + w < SCREEN_W) {
                    mvaddch(y, p->x + w, '|');
                }
            }
        }
        
        // 下方管道
        for (int y = p->gap_y + PIPE_GAP; y < SCREEN_H - 2; y++) {
            for (int w = 0; w < PIPE_WIDTH; w++) {
                if (p->x + w >= 0 && p->x + w < SCREEN_W) {
                    mvaddch(y, p->x + w, '|');
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
            mvprintw(SCREEN_H / 2 - 2, SCREEN_W / 2 - 10, "FLAPPY BIRD");
            mvprintw(SCREEN_H / 2, SCREEN_W / 2 - 15, "Press SPACE to Start");
            mvprintw(SCREEN_H / 2 + 2, SCREEN_W / 2 - 15, "High Score: %d", g_highscore);
            break;
            
        case SCENE_READY:
            mvprintw(2, SCREEN_W / 2 - 10, "Get Ready!");
            mvprintw(4, SCREEN_W / 2 - 15, "Press SPACE to Jump");
            mvprintw(SCREEN_H - 4, 2, "Score: %d", g_score);
            break;
            
        case SCENE_GAME:
            mvprintw(1, 2, "Score: %d", g_score);
            break;
            
        case SCENE_OVER:
            mvprintw(SCREEN_H / 2 - 3, SCREEN_W / 2 - 8, "GAME OVER");
            mvprintw(SCREEN_H / 2 - 1, SCREEN_W / 2 - 10, "Score: %d", g_score);
            mvprintw(SCREEN_H / 2 + 1, SCREEN_W / 2 - 10, "Best: %d", g_highscore);
            mvprintw(SCREEN_H / 2 + 3, SCREEN_W / 2 - 15, "Press SPACE to Restart");
            mvprintw(SCREEN_H / 2 + 5, SCREEN_W / 2 - 10, "Press Q to Quit");
            break;
    }
    
    attroff(COLOR_PAIR(4) | A_BOLD);
}