#ifndef INPUT_H
#define INPUT_H

// 关闭终端行缓冲，实现无阻塞读按键
void input_init(void);
// 获取按键，无按键返回0
int input_get_key(void);
// 恢复终端默认模式
void input_restore(void);
#endif
