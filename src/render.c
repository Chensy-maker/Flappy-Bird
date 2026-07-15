#include "render.h"
#include "game.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ============ 全局纹理 ============ */
SDL_Window   *g_window        = NULL;
SDL_Texture  *tex_bg          = NULL;
SDL_Texture  *tex_ground      = NULL;
SDL_Texture  *tex_bird[9]     = {NULL, NULL, NULL};
SDL_Texture  *tex_pipe_down   = NULL;
SDL_Texture  *tex_pipe_up     = NULL;
SDL_Texture  *tex_title       = NULL;
SDL_Texture  *tex_ready       = NULL;
SDL_Texture  *tex_game_over   = NULL;
SDL_Texture  *tex_tutorial    = NULL;
SDL_Texture  *tex_btn_pause   = NULL;
SDL_Texture  *tex_btn_resume  = NULL;
SDL_Texture  *tex_numbers[10] = {NULL};
SDL_Texture  *tex_score_panel = NULL;
SDL_Texture  *tex_new         = NULL;
SDL_Texture  *tex_button_score   = NULL;
SDL_Texture  *tex_font_numbers[10] = {NULL};

/* 尺寸缓存 */
static int bg_w = 0, bg_h = 0;
static int ground_w = 0, ground_h = 0;
static int pipe_w = 0, pipe_h = 0;
static int num_w = 0, num_h = 0;
static int font_w = 0, font_h = 0;

/* ============ 工具函数 ============ */

static SDL_Texture *load_tex(const char *path)
{
    SDL_Texture *tex = IMG_LoadTexture(g_renderer, path);
    if (!tex)
        fprintf(stderr, "Failed to load %s: %s\n", path, IMG_GetError());
    return tex;
}

static void get_tex_size(SDL_Texture *tex, int *w, int *h)
{
    if (tex) SDL_QueryTexture(tex, NULL, NULL, w, h);
}

if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
fprintf(stderr, "IMG_Init failed: %s\n", IMG_GetError());
SDL_Quit();
return -1;
}

g_window = SDL_CreateWindow(
"Flappy Bird",
SDL_WINDOWPOS_CENTERED,
SDL_WINDOWPOS_CENTERED,
SCREEN_WIDTH, SCREEN_HEIGHT,
SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
);

if (!g_window) {
fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
IMG_Quit();
SDL_Quit();
return -1;
}

g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

if (!g_renderer) {
fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
SDL_DestroyWindow(g_window);
IMG_Quit();
SDL_Quit();
return -1;
}

SDL_RenderSetLogicalSize(g_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);


// 可选：限制窗口最小尺寸，防止缩到过小看不见内容
SDL_SetWindowMinimumSize(g_window, SCREEN_WIDTH, SCREEN_HEIGHT);


tex_bg_day = load_texture("bg_day.png");
tex_land = load_texture("land.png");

tex_bird[0] = load_texture("bird0_0.png");
tex_bird[1] = load_texture("bird0_1.png");
tex_bird[2] = load_texture("bird0_2.png");

tex_pipe_up = load_texture("pipe_up.png");
tex_pipe_down = load_texture("pipe_down.png");

for (int i = 0; i < 10; i++) {
char filename[64];
snprintf(filename, sizeof(filename), "number_score_%02d.png", i);
tex_number[i] = load_texture(filename);
}

tex_title = load_texture("title.png");
tex_text_ready = load_texture("text_ready.png");
tex_text_gameover = load_texture("text_game_over.png");
tex_tutorial = load_texture("tutorial.png");
tex_score_panel = load_texture("score_panel.png");
tex_new_mark = load_texture("new.png");
tex_btn_play = load_texture("button_play.png");
tex_btn_ok = load_texture("button_ok.png");

printf("Render system initialized\n");
return 0;
}

// ============ 清理 ============
void render_quit(void)
static void draw_centered(SDL_Texture *tex, int cx, int cy, int tw, int th)
{
    if (!tex) return;
    SDL_Rect dest = {cx - tw / 2, cy - th / 2, tw, th};
    SDL_RenderCopy(g_renderer, tex, NULL, &dest);
}

static void draw_number_right(int number, int rx, int cy, int dw)
{
    if (number < 0) number = 0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", number);
    int len = (int)strlen(buf);
    int sx = rx - len * dw;
    for (int i = 0; i < len; i++) {
        int d = buf[i] - '0';
        if (d >= 0 && d <= 9 && tex_numbers[d]) {
            SDL_Rect dest = {sx + i * dw, cy - num_h / 2, dw, num_h};
            SDL_RenderCopy(g_renderer, tex_numbers[d], NULL, &dest);
        }
    }
}

static void draw_number_center(int number, int cx, int cy, int dw)
{
    if (number < 0) number = 0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", number);
    int len = (int)strlen(buf);
    int sx = cx - (len * dw) / 2;
    for (int i = 0; i < len; i++) {
        int d = buf[i] - '0';
        if (d >= 0 && d <= 9 && tex_numbers[d]) {
            SDL_Rect dest = {sx + i * dw, cy - num_h / 2, dw, num_h};
            SDL_RenderCopy(g_renderer, tex_numbers[d], NULL, &dest);
        }
    }
}

static void draw_font_number_center(int number, int cx, int cy, int dw)
{
    if (number < 0) number = 0;
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", number);
    int len = (int)strlen(buf);
    int sx = cx - (len * dw) / 2;
    for (int i = 0; i < len; i++) {
        int d = buf[i] - '0';
        if (d >= 0 && d <= 9 && tex_font_numbers[d]) {
            SDL_Rect dest = {sx + i * dw, cy - font_h / 2, dw, font_h};
            SDL_RenderCopy(g_renderer, tex_font_numbers[d], NULL, &dest);
        }
    }
}

/* ============ 场景元素绘制 ============ */

static void draw_background(void)
{
    if (!tex_bg) return;
    if (bg_w == 0) get_tex_size(tex_bg, &bg_w, &bg_h);
    if (bg_w <= 0) bg_w = 288;
    if (bg_h <= 0) bg_h = 512;

    int off = (int)g_bg_offset % bg_w;
    if (off > 0) off -= bg_w;

    for (int x = off; x < SCREEN_W; x += bg_w) {
        int dw = bg_w;
        if (x + dw > SCREEN_W) dw = SCREEN_W - x;
        SDL_Rect src  = {0, 0, dw, bg_h};
        SDL_Rect dest = {x, 0, dw, bg_h};
        SDL_RenderCopy(g_renderer, tex_bg, &src, &dest);
    }
}

static void draw_ground(void)
{
    if (!tex_ground) return;
    if (ground_w == 0) get_tex_size(tex_ground, &ground_w, &ground_h);
    if (ground_w <= 0) ground_w = 336;
    if (ground_h <= 0) ground_h = 112;

    int gy = SCREEN_H - ground_h;
    int off = (int)g_ground_offset % ground_w;
    if (off > 0) off -= ground_w;

    for (int x = off; x < SCREEN_W; x += ground_w) {
        int dw = ground_w;
        if (x + dw > SCREEN_W) dw = SCREEN_W - x;
        SDL_Rect src  = {0, 0, dw, ground_h};
        SDL_Rect dest = {x, gy, dw, ground_h};
        SDL_RenderCopy(g_renderer, tex_ground, &src, &dest);
    }
}

static void draw_pipe_top(float px, int gap_top)
{
    if (!tex_pipe_down) return;
    if (pipe_w == 0 || pipe_h == 0)
        get_tex_size(tex_pipe_down, &pipe_w, &pipe_h);
    if (pipe_w <= 0) pipe_w = 52;
    if (pipe_h <= 0) pipe_h = 320;

    int body_src_h = pipe_h - PIPE_HEAD_H;
    if (gap_top <= 0) return;

    if (gap_top <= PIPE_HEAD_H) {
        SDL_Rect src  = {0, pipe_h - gap_top, pipe_w, gap_top};
        SDL_Rect dest = {(int)px, 0, pipe_w, gap_top};
        SDL_RenderCopyEx(g_renderer, tex_pipe_down, &src, &dest, 180.0, NULL, SDL_FLIP_NONE);
    } else {
        SDL_Rect sb = {0, 0, pipe_w, body_src_h};
        SDL_Rect db = {(int)px, 0, pipe_w, gap_top - PIPE_HEAD_H};
        SDL_RenderCopyEx(g_renderer, tex_pipe_down, &sb, &db, 180.0, NULL, SDL_FLIP_NONE);

        SDL_Rect sh = {0, pipe_h - PIPE_HEAD_H, pipe_w, PIPE_HEAD_H};
        SDL_Rect dh = {(int)px, gap_top - PIPE_HEAD_H, pipe_w, PIPE_HEAD_H};
        SDL_RenderCopyEx(g_renderer, tex_pipe_down, &sh, &dh, 180.0, NULL, SDL_FLIP_NONE);
    }
}

static void draw_pipe_bottom(float px, int gap_bottom, int ground_y)
{
    if (!tex_pipe_up) return;
    if (pipe_w == 0 || pipe_h == 0)
        get_tex_size(tex_pipe_up, &pipe_w, &pipe_h);
    if (pipe_w <= 0) pipe_w = 52;
    if (pipe_h <= 0) pipe_h = 320;

    int body_src_h = pipe_h - PIPE_HEAD_H;
    int ph = ground_y - gap_bottom;
    if (ph <= 0) return;

    if (ph <= PIPE_HEAD_H) {
        SDL_Rect src  = {0, 0, pipe_w, ph};
        SDL_Rect dest = {(int)px, gap_bottom, pipe_w, ph};
        SDL_RenderCopy(g_renderer, tex_pipe_up, &src, &dest);
    } else {
        SDL_Rect sh = {0, 0, pipe_w, PIPE_HEAD_H};
        SDL_Rect dh = {(int)px, gap_bottom, pipe_w, PIPE_HEAD_H};
        SDL_RenderCopy(g_renderer, tex_pipe_up, &sh, &dh);

        SDL_Rect sb = {0, PIPE_HEAD_H, pipe_w, body_src_h};
        SDL_Rect db = {(int)px, gap_bottom + PIPE_HEAD_H,
                       pipe_w, ph - PIPE_HEAD_H};
        SDL_RenderCopy(g_renderer, tex_pipe_up, &sb, &db);
    }
}

static void draw_pipes(void)
{
    if (!tex_pipe_down || !tex_pipe_up) return;
    int ground_y = SCREEN_H - GROUND_HEIGHT;
    Pipe *p = g_pipes;
    while (p) {
        if (p->x + 52 < -50 || p->x > SCREEN_W + 50) {
            p = p->next;
            continue;
        }
        int gap_top    = p->gap_center - PIPE_GAP / 2;
        int gap_bottom = p->gap_center + PIPE_GAP / 2;
        draw_pipe_top(p->x, gap_top);
        draw_pipe_bottom(p->x, gap_bottom, ground_y);
        p = p->next;
    }
}

static void draw_bird(void)
{
    int frame = g_bird.frame;
    if (frame < 0 || frame > 2) frame = 1;
    // 根据颜色+帧计算纹理下标
    int bird_idx = g_bird_color * 3 + frame;
    // 边界保护，防止越界
    if (bird_idx < 0 || bird_idx >= 9) bird_idx = frame;
    SDL_Texture *tex = tex_bird[bird_idx];

    if (!tex) return;

    SDL_Rect dest = {(int)BIRD_X, (int)g_bird.y, BIRD_W, BIRD_H};
    SDL_RenderCopy(g_renderer, tex, NULL, &dest);
}

/* ============ UI 绘制 ============ */

static void draw_start_ui(void)
{
    if (tex_title) {
    int tw = 178, th = 48;
    SDL_QueryTexture(tex_title, NULL, NULL, &tw, &th);
    Uint32 tick = SDL_GetTicks();
    float offset_y = sinf(tick * 0.003f) * 8.0f;
    int title_y = (int)(SCREEN_H / 3 + offset_y);
    draw_centered(tex_title, SCREEN_W / 2, title_y, tw, th);
}
    if (g_highscore > 0 && tex_numbers[0]) {
        if (num_h == 0) get_tex_size(tex_numbers[0], &num_w, &num_h);
        if (num_h <= 0) { num_w = 20; num_h = 28; }
        draw_number_center(g_highscore, SCREEN_W / 2, SCREEN_H / 2 -10, num_w);
    }
    if (tex_button_score) {
        Uint32 ticks = SDL_GetTicks();
        float y_offset = (float)sin(ticks * 0.003) * 5.0f;
        int sw = 0, sh = 0;
        SDL_QueryTexture(tex_button_score, NULL, NULL, &sw, &sh);
        if (sw <= 0) { sw = 120; sh = 40; }
        int num_display_h = num_h > 0 ? num_h : 28;
        int by = SCREEN_H / 2  + num_display_h/2 + sh/2  + (int)y_offset;
        draw_centered(tex_button_score, SCREEN_W / 2, by, sw, sh);
    }
}

static void draw_ready_ui(void)
{
    if (tex_ready) {
        int rw = 174, rh = 44;
        SDL_QueryTexture(tex_ready, NULL, NULL, &rw, &rh);
        draw_centered(tex_ready, SCREEN_W / 2, SCREEN_H / 4, rw, rh);
    }
    if (tex_numbers[0] && g_countdown > 0) {
        if (num_h == 0) get_tex_size(tex_numbers[0], &num_w, &num_h);
        if (num_h <= 0) { num_w = 36; num_h = 48; }
        draw_number_center(g_countdown, SCREEN_W / 2, SCREEN_H / 4 + 70, num_w);
    }
    /* --- 倒计时下方：教程图片，上下律动 --- */
    if (tex_tutorial) {
        Uint32 tick = SDL_GetTicks();
        float y_offset = sinf(tick * 0.003f) * 8.0f;
        int tw = 0, th = 0;
        SDL_QueryTexture(tex_tutorial, NULL, NULL, &tw, &th);
        if (tw <= 0) { tw = 80; th = 80; }
        /* 定位在倒计时数字下方约 20px */
        int count_h = (num_h > 0) ? num_h : 48;
        int ty = SCREEN_H / 4 + 70 + count_h / 2 + 20 + th / 2 + (int)y_offset;
        draw_centered(tex_tutorial, SCREEN_W / 2, ty, tw, th);
    }
}

static void draw_pause_overlay(void)
{
    /* 半透明黑遮罩 */
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, SCREEN_W, SCREEN_H};
    SDL_RenderFillRect(g_renderer, &overlay);
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_NONE);
}

static void draw_pause_button(void)
{
    if (!tex_btn_pause) return;
    int pw = PAUSE_BTN_W, ph = PAUSE_BTN_H;
    int rw = 0, rh = 0;
    SDL_QueryTexture(tex_btn_pause, NULL, NULL, &rw, &rh);
    if (rw > 0) { pw = rw; ph = rh; }
    SDL_Rect dest = {PAUSE_BTN_X, PAUSE_BTN_Y, pw, ph};
    SDL_RenderCopy(g_renderer, tex_btn_pause, NULL, &dest);
}

static void draw_resume_button(void)
{
    if (!tex_btn_resume) return;
    int bw = 0, bh = 0;
    SDL_QueryTexture(tex_btn_resume, NULL, NULL, &bw, &bh);
    if (bw <= 0) { bw = RESUME_BTN_W; bh = RESUME_BTN_H; }
    draw_centered(tex_btn_resume, SCREEN_W / 2, SCREEN_H / 2, bw, bh);
}

static void draw_score_ui(void)
{
    if (!tex_font_numbers[0]) return;
    if (font_h == 0) get_tex_size(tex_font_numbers[0], &font_w, &font_h);
    if (font_h <= 0) { font_w = 24; font_h = 36; }
    draw_font_number_center(g_score, SCREEN_W / 2, 55, font_w);
}

static void draw_over_ui(void)
{
    if (!tex_game_over) return;
    int cx = SCREEN_W / 2;
    int gw = 192, gh = 42;
    SDL_QueryTexture(tex_game_over, NULL, NULL, &gw, &gh);
    draw_centered(tex_game_over, cx, SCREEN_H / 3, gw, gh);

    if (tex_score_panel) {
        int pw = 226, ph = 112;
        SDL_QueryTexture(tex_score_panel, NULL, NULL, &pw, &ph);
        SDL_Rect pd = {cx - pw / 2, SCREEN_H / 3 + 40, pw, ph};
        SDL_RenderCopy(g_renderer, tex_score_panel, NULL, &pd);

        if (num_h == 0 && tex_numbers[0])
            get_tex_size(tex_numbers[0], &num_w, &num_h);
        if (num_h <= 0) { num_w = 20; num_h = 28; }

        int sy = SCREEN_H / 3 + 40 + ph / 2 - 20;
        int hy = SCREEN_H / 3 + 40 + ph / 2 + 30;
        draw_number_right(g_score, cx + pw / 2 - 25, sy, num_w);
        draw_number_right(g_highscore, cx + pw / 2 - 25, hy, num_w);

        if (g_score > 0 && g_score >= g_highscore && tex_new) {
            int nw = 32, nh = 20;
            SDL_QueryTexture(tex_new, NULL, NULL, &nw, &nh);
            char buf[16];
            snprintf(buf, sizeof(buf), "%d", g_score);
            int sw = (int)strlen(buf) * num_w;
            SDL_Rect nd = {cx + pw / 2 - 20 - sw - nw - 5, sy - nh / 2, nw, nh};
            SDL_RenderCopy(g_renderer, tex_new, NULL, &nd);
        }
    }
}

/* ============ 主绘制 ============ */

void render_draw(void)
{
    SDL_SetRenderDrawColor(g_renderer, 80, 160, 200, 255);
    SDL_RenderClear(g_renderer);

    draw_background();
    draw_ground();
    draw_pipes();
    draw_bird();

    switch (g_scene) {
    case SCENE_START: draw_start_ui(); break;
    case SCENE_READY: draw_ready_ui(); draw_score_ui(); break;
    case SCENE_GAME:
        draw_score_ui();
        draw_pause_button();          /* 左上角暂停按钮 */
        if (g_paused) {
            draw_pause_overlay();     /* 半透明黑遮罩 */
            draw_resume_button();     /* 中间继续按钮 */
        }
        break;
    case SCENE_OVER:  draw_over_ui(); break;
    }

    SDL_RenderPresent(g_renderer);
}

/* ============ 初始化 ============ */

int render_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "IMG_Init error: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    g_window = SDL_CreateWindow(
        "Flappy Bird",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H, 0
    );
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        IMG_Quit(); SDL_Quit();
        return -1;
    }

    g_renderer = SDL_CreateRenderer(
        g_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!g_renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        IMG_Quit(); SDL_Quit();
        return -1;
    }

    tex_bg        = load_tex("assets/images/bg_day.png");
    tex_ground    = load_tex("assets/images/land.png");
    tex_bird[0]   = load_tex("assets/images/bird0_0.png");
    tex_bird[1]   = load_tex("assets/images/bird0_1.png");
    tex_bird[2]   = load_tex("assets/images/bird0_2.png");
    tex_bird[3]   = load_tex("assets/images/bird1_0.png");
    tex_bird[4]   = load_tex("assets/images/bird1_1.png");
    tex_bird[5]   = load_tex("assets/images/bird1_2.png");
    tex_bird[6]   = load_tex("assets/images/bird2_0.png");
    tex_bird[7]   = load_tex("assets/images/bird2_1.png");
    tex_bird[8]   = load_tex("assets/images/bird2_2.png");
    tex_pipe_down = load_tex("assets/images/pipe_down.png");
    tex_pipe_up   = load_tex("assets/images/pipe_up.png");
    tex_title     = load_tex("assets/images/title.png");
    tex_ready     = load_tex("assets/images/text_ready.png");
    tex_game_over = load_tex("assets/images/text_game_over.png");
    tex_tutorial  = load_tex("assets/images/tutorial.png");
    tex_btn_pause = load_tex("assets/images/button_pause.png");
    tex_score_panel = load_tex("assets/images/score_panel.png");
    tex_new       = load_tex("assets/images/new.png");

    char path[64];
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "assets/images/number_score_0%d.png", i);
        tex_numbers[i] = load_tex(path);
    }

    tex_btn_resume   = load_tex("assets/images/button_resume.png");
    tex_button_score = load_tex("assets/images/button_score.png");
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "assets/images/font_%03d.png", 48 + i);
        tex_font_numbers[i] = load_tex(path);
    }

    return 0;
}

void render_quit(void)
{
    for (int i = 0; i < 9; i++)
        if (tex_bird[i]) SDL_DestroyTexture(tex_bird[i]);
    for (int i = 0; i < 10; i++)
        if (tex_numbers[i]) SDL_DestroyTexture(tex_numbers[i]);

    if (tex_btn_pause)    SDL_DestroyTexture(tex_btn_pause);
    if (tex_btn_resume)   SDL_DestroyTexture(tex_btn_resume);
    if (tex_button_score) SDL_DestroyTexture(tex_button_score);
    for (int i = 0; i < 10; i++)
        if (tex_font_numbers[i]) SDL_DestroyTexture(tex_font_numbers[i]);

    if (tex_tutorial)    SDL_DestroyTexture(tex_tutorial);
    if (tex_bg)          SDL_DestroyTexture(tex_bg);
    if (tex_ground)      SDL_DestroyTexture(tex_ground);
    if (tex_pipe_down)   SDL_DestroyTexture(tex_pipe_down);
    if (tex_pipe_up)     SDL_DestroyTexture(tex_pipe_up);
    if (tex_title)       SDL_DestroyTexture(tex_title);
    if (tex_ready)       SDL_DestroyTexture(tex_ready);
    if (tex_game_over)   SDL_DestroyTexture(tex_game_over);
    if (tex_score_panel) SDL_DestroyTexture(tex_score_panel);
    if (tex_new)         SDL_DestroyTexture(tex_new);

    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window)   SDL_DestroyWindow(g_window);
    IMG_Quit();
    SDL_Quit();
}
