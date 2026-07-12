# Flappy Bird C 终端版
基于ncurses终端图形库实现的Flappy Bird小游戏，纯C语言，有限状态机游戏流程。
## 环境依赖
Ubuntu
sudo apt update
sudo apt install build-essential git ncurses-dev
## 编译运行
make clean
make
make run
## 操作说明
空格：开始游戏 / 小鸟跳跃 / 重新开始
ESC：退出游戏
## 项目结构
include/ 头文件定义
src/     功能实现
data/    最高分持久化存储
