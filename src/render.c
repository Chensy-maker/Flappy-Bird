#include "render.h"
#include "game.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* ============ ?????? ============ */
SDL_Window   *g_window        = NULL;
SDL_Texture  *tex_bg          = NULL;
SDL_Texture  *tex_ground      = NULL;
SDL_Texture  *tex_bird[3]     = {NULL, NULL, NULL};
SDL_Texture  *tex_pipe_down   = NULL;
SDL_Texture  *tex_pipe_up     = NULL;
SDL_Texture  *tex_title       = NULL;
SDL_Texture  *tex_ready       = NULL;
SDL_Texture  *tex_game_over   = NULL;
SDL_Texture  *tex_numbers[10] = {NULL};
SDL_Texture  *tex_score_panel = NULL;
SDL_Texture  *tex_new         = NULL;

/* ?????? */
static int bg_w = 0, bg_h = 0;
static int ground_w = 0, ground_h = 0;
static int pipe_w = 0, pipe_h = 0;
static int num_w = 0, num_h = 0;

/* ============ ???? ============ */

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

/* ============ ???? ============ */

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
        SDL_RenderCopy(g_renderer, tex_pipe_down, &src, &dest);
    } else {
        SDL_Rect sb = {0, 0, pipe_w, body_src_h};
        SDL_Rect db = {(int)px, 0, pipe_w, gap_top - PIPE_HEAD_H};
        SDL_RenderCopy(g_renderer, tex_pipe_down, &sb, &db);

        SDL_Rect sh = {0, pipe_h - PIPE_HEAD_H, pipe_w, PIPE_HEAD_H};
        SDL_Rect dh = {(int)px, gap_top - PIPE_HEAD_H, pipe_w, PIPE_HEAD_H};
        SDL_RenderCopy(g_renderer, tex_pipe_down, &sh, &dh);
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
    SDL_Texture *tex = tex_bird[frame];
    if (!tex) return;

    SDL_Rect dest = {(int)BIRD_X, (int)g_bird.y, BIRD_W, BIRD_H};
    SDL_RenderCopy(g_renderer, tex, NULL, &dest);
}

/* ============ UI?? ============ */

static void draw_start_ui(void)
{
    if (tex_title) {
        int tw = 178, th = 48;
        SDL_QueryTexture(tex_title, NULL, NULL, &tw, &th);
        draw_centered(tex_title, SCREEN_W / 2, SCREEN_H / 3, tw, th);
    }
    if (g_highscore > 0 && tex_numbers[0]) {
        if (num_h == 0) get_tex_size(tex_numbers[0], &num_w, &num_h);
        if (num_h <= 0) { num_w = 20; num_h = 28; }
        draw_number_center(g_highscore, SCREEN_W / 2, SCREEN_H / 2 + 40, num_w);
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
}

static void draw_score_ui(void)
{
    if (!tex_numbers[0]) return;
    if (num_h == 0) get_tex_size(tex_numbers[0], &num_w, &num_h);
    if (num_h <= 0) { num_w = 24; num_h = 36; }
    draw_number_center(g_score, SCREEN_W / 2, 55, num_w);
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

        int sy = SCREEN_H / 3 + 40 + ph / 2 - 30;
        int hy = SCREEN_H / 3 + 40 + ph / 2 + 5;
        draw_number_right(g_score, cx + pw / 2 - 20, sy, num_w);
        draw_number_right(g_highscore, cx + pw / 2 - 20, hy, num_w);

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

/* ============ ????? ============ */

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
    case SCENE_GAME:  draw_score_ui(); break;
    case SCENE_OVER:  draw_over_ui(); break;
    }

    SDL_RenderPresent(g_renderer);
}

/* ============ ?????? ============ */

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
    tex_pipe_down = load_tex("assets/images/pipe_down.png");
    tex_pipe_up   = load_tex("assets/images/pipe_up.png");
    tex_title     = load_tex("assets/images/title.png");
    tex_ready     = load_tex("assets/images/text_ready.png");
    tex_game_over = load_tex("assets/images/text_game_over.png");
    tex_score_panel = load_tex("assets/images/score_panel.png");
    tex_new       = load_tex("assets/images/new.png");

    char path[64];
    for (int i = 0; i < 10; i++) {
        snprintf(path, sizeof(path), "assets/images/number_score_0%d.png", i);
        tex_numbers[i] = load_tex(path);
    }

    return 0;
}

void render_quit(void)
{
    for (int i = 0; i < 3; i++)
        if (tex_bird[i]) SDL_DestroyTexture(tex_bird[i]);
    for (int i = 0; i < 10; i++)
        if (tex_numbers[i]) SDL_DestroyTexture(tex_numbers[i]);

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
