#ifndef RENDER_H
#define RENDER_H
// 渲染模块接口
int  render_init(void);    // 初始化SDL和加载资源
void render_quit(void);    // 清理SDL资源
void render_draw(void);    // 绘制当前帧
#endif
