
#include "render.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


#define ASSETS_PATH   "assets/images/"

// ============ SDL核心 ============
SDL_Window   *g_window   = NULL;
SDL_Renderer *g_renderer = NULL;

// ============ 纹理资源 ============
static SDL_Texture *tex_bg_day       = NULL;
static SDL_Texture *tex_land         = NULL;
static SDL_Texture *tex_bird[3]      = {NULL};
static SDL_Texture *tex_pipe_up      = NULL;
static SDL_Texture *tex_pipe_down    = NULL;
static SDL_Texture *tex_number[10]   = {NULL};
static SDL_Texture *tex_title        = NULL;
static SDL_Texture *tex_text_ready   = NULL;
static SDL_Texture *tex_text_gameover= NULL;
static SDL_Texture *tex_tutorial     = NULL;
static SDL_Texture *tex_score_panel  = NULL;
static SDL_Texture *tex_new_mark     = NULL;
static SDL_Texture *tex_btn_play     = NULL;
static SDL_Texture *tex_btn_ok       = NULL;

// ============ 滚动状态 ============
static float scroll_bg   = 0.0f;
static float scroll_land = 0.0f;

// ============ 加载纹理 ============
static SDL_Texture* load_texture(const char *filename)
{
char path[256];
snprintf(path, sizeof(path), "%s%s", ASSETS_PATH, filename);
SDL_Texture *tex = IMG_LoadTexture(g_renderer, path);
if (!tex) {
fprintf(stderr, "Failed to load: %s\n", filename);
}
return tex;
}

// ============ 初始化 ============
int render_init(void)
{
if (SDL_Init(SDL_INIT_VIDEO) < 0) {
fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
return -1;
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
{
#define DESTROY(tex) if(tex) { SDL_DestroyTexture(tex); tex=NULL; }

DESTROY(tex_bg_day);
DESTROY(tex_land);
for (int i = 0; i < 3; i++) DESTROY(tex_bird[i]);
DESTROY(tex_pipe_up);
DESTROY(tex_pipe_down);
for (int i = 0; i < 10; i++) DESTROY(tex_number[i]);
DESTROY(tex_title);
DESTROY(tex_text_ready);
DESTROY(tex_text_gameover);
DESTROY(tex_tutorial);
DESTROY(tex_score_panel);
DESTROY(tex_new_mark);
DESTROY(tex_btn_play);
DESTROY(tex_btn_ok);

if (g_renderer) SDL_DestroyRenderer(g_renderer);
if (g_window) SDL_DestroyWindow(g_window);
IMG_Quit();
SDL_Quit();
}

// ============ 绘制数字 ============
static void draw_number(int score, int center_x, int y)
{
if (!tex_number[0]) return;

const int digit_w = 18;
const int digit_h = 24;

if (score == 0) {
SDL_Rect rect = {center_x - digit_w/2, y, digit_w, digit_h};
SDL_RenderCopy(g_renderer, tex_number[0], NULL, &rect);
return;
}

int digits[16];
int count = 0;
int temp = score;
while (temp > 0) {
digits[count++] = temp % 10;
temp /= 10;
}

int total_w = count * digit_w;
int start_x = center_x - total_w / 2;

for (int i = count - 1; i >= 0; i--) {
SDL_Rect rect = {start_x, y, digit_w, digit_h};
SDL_RenderCopy(g_renderer, tex_number[digits[i]], NULL, &rect);
start_x += digit_w;
}
}

// ============ 绘制背景 ============
static void draw_background(void)
{
if (!tex_bg_day) return;

if (g_scene == SCENE_GAME) {
scroll_bg -= 0.5f;
if (scroll_bg <= -SCREEN_WIDTH) scroll_bg = 0;
}

SDL_Rect r1 = {(int)scroll_bg, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect r2 = {(int)scroll_bg + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_RenderCopy(g_renderer, tex_bg_day, NULL, &r1);
SDL_RenderCopy(g_renderer, tex_bg_day, NULL, &r2);
}

// ============ 绘制地面 ============
static void draw_ground(void)
{
if (!tex_land) return;

if (g_scene == SCENE_GAME) {
scroll_land -= PIPE_SPEED;
if (scroll_land <= -336) scroll_land = 0;
}

int ground_y = SCREEN_HEIGHT - 112;
SDL_Rect r1 = {(int)scroll_land,       ground_y, 336, 112};
SDL_Rect r2 = {(int)scroll_land + 336, ground_y, 336, 112};
SDL_RenderCopy(g_renderer, tex_land, NULL, &r1);
SDL_RenderCopy(g_renderer, tex_land, NULL, &r2);
}

// ============ 绘制小鸟 ============
static void draw_bird(void)
{
if (!tex_bird[0]) return;

float angle = g_bird.vy * 3.0f;
if (angle < -30) angle = -30;
if (angle > 90)  angle = 90;

int render_x = BIRD_X * SCREEN_WIDTH / SCREEN_W;
int render_y = (int)g_bird.y * SCREEN_HEIGHT / SCREEN_H;

SDL_Rect dst = {render_x - BIRD_W/2, render_y - BIRD_H/2, BIRD_W, BIRD_H};
int frame = g_bird.frame % 3;
SDL_RenderCopyEx(g_renderer, tex_bird[frame], NULL, &dst, angle, NULL, SDL_FLIP_NONE);
}

// ============ 绘制管道 ============
static void draw_pipes(void)
{
if (!tex_pipe_up || !tex_pipe_down) return;

const int pipe_w = 52;
const int pipe_h = 320;

Pipe *p = g_pipes;
while (p) {
int render_x = (int)p->x * SCREEN_WIDTH / SCREEN_W;
int render_gap_y = p->gap_y * SCREEN_HEIGHT / SCREEN_H;
int render_gap = PIPE_GAP * SCREEN_HEIGHT / SCREEN_H;

SDL_Rect top = {render_x, render_gap_y - render_gap/2 - pipe_h, pipe_w, pipe_h};
SDL_RenderCopy(g_renderer, tex_pipe_down, NULL, &top);

SDL_Rect bottom = {render_x, render_gap_y + render_gap/2, pipe_w, pipe_h};
SDL_RenderCopy(g_renderer, tex_pipe_up, NULL, &bottom);

p = p->next;
}
}

// ============ 主渲染 ============
void render_draw(void)
{
if (!g_renderer) return;

SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
SDL_RenderClear(g_renderer);

draw_background();

switch (g_scene) {
case SCENE_START:
draw_bird();
draw_ground();
if (tex_title) {
SDL_Rect r = {SCREEN_WIDTH/2 - 89, 60, 178, 48};
SDL_RenderCopy(g_renderer, tex_title, NULL, &r);
}
if (tex_btn_play) {
SDL_Rect r = {SCREEN_WIDTH/2 - 40, 300, 80, 28};
SDL_RenderCopy(g_renderer, tex_btn_play, NULL, &r);
}
break;

case SCENE_READY:
draw_bird();
draw_ground();
if (tex_text_ready) {
SDL_Rect r = {SCREEN_WIDTH/2 - 92, 100, 184, 50};
SDL_RenderCopy(g_renderer, tex_text_ready, NULL, &r);
}
if (tex_tutorial) {
SDL_Rect r = {SCREEN_WIDTH/2 - 57, 200, 114, 98};
SDL_RenderCopy(g_renderer, tex_tutorial, NULL, &r);
}
break;

case SCENE_GAME:
draw_pipes();
draw_bird();
draw_ground();
draw_number(g_score, SCREEN_WIDTH/2, 50);
break;

case SCENE_OVER:
draw_pipes();
draw_bird();
draw_ground();
if (tex_text_gameover) {
SDL_Rect r = {SCREEN_WIDTH/2 - 102, 80, 204, 54};
SDL_RenderCopy(g_renderer, tex_text_gameover, NULL, &r);
}
if (tex_score_panel) {
SDL_Rect r = {SCREEN_WIDTH/2 - 119, 150, 238, 126};
SDL_RenderCopy(g_renderer, tex_score_panel, NULL, &r);

draw_number(g_score, SCREEN_WIDTH/2 + 70, 175);
draw_number(g_highscore, SCREEN_WIDTH/2 + 70, 220);

if (g_score > g_highscore && tex_new_mark) {
SDL_Rect r = {SCREEN_WIDTH/2 + 10, 215, 32, 14};
SDL_RenderCopy(g_renderer, tex_new_mark, NULL, &r);
}
}
if (tex_btn_ok) {
SDL_Rect r = {SCREEN_WIDTH/2 - 40, 320, 80, 28};
SDL_RenderCopy(g_renderer, tex_btn_ok, NULL, &r);
}
break;
}

SDL_RenderPresent(g_renderer);
}