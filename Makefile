#指定使用gcc作为c语言编译器
CC      = gcc
#C编程参数
CFLAGS  = -I include -Wall -Wextra -g -O2 $(shell sdl2-config --cflags)
# 参数拆分解释：
# -I include ：告诉编译器头文件在 include/ 文件夹，#include "xxx.h"才能找到
# -Wall      开启基础警告，代码不规范会提示
# -Wextra    开启更多严格警告，方便找bug
# -g         加入调试信息，gdb调试程序必备
# -O2        二级代码优化，游戏运行更流畅
# $(shell sdl2-config --cflags) 自动读取SDL2需要的头文件编译参数

#链接参数，所有.o合并可执行程序的时候使用
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lm
# $(shell sdl2-config --libs) 自动获取SDL2基础链接库
# -lSDL2\_image 加载png图片纹理依赖库
# -lSDL2\_mixer 播放ogg音效依赖库
# -lm 数学库，数学函数必须链接


# 所有C源码文件列表，全部放在src目录下
SRC     = src/main.c src/game.c src/render.c src/physics.c src/input.c src/audio.c
# 存放编译中间文件(.o)的文件夹名
OBJ_DIR = obj
#函数 `patsubst` 将SRC中所有的src/%.c替换为obj/%.o，生成对应的.o文件列表,赋值给OBJ变量
OBJ     = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

# 最终生成的游戏可执行程序的名称，输出在项目根目录
TARGET  = flappy_bird

#输入make等价于make all，最终生成flappy_bird可执行程序
#依赖$(TARGET)，所以会自动执行flappy_bird的编译规则
all: $(TARGET)


# 生成$(TARGET)即flappybird，依赖所有obj/*.o 文件
$(TARGET): $(OBJ)
	# 执行gcc链接命令：所有.o文件合并，输出程序，带上SDL等链接库
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# 编译规则：src下c文件编译到obj文件夹
$(OBJ_DIR)/%.o: src/%.c
	# 文件夹不存在就自动创建
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	#$<代表当前规则的依赖文件(src/xxx.c)
	#$@代表当前规则的目标文件(obj/xxx.o)


# 清理：删除obj整个文件夹 + 可执行文件
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

#依赖$(TARGET)，执行./$(TARGET)运行游戏
run: $(TARGET)
	./$(TARGET)

#声明all clean run为伪目标，避免与同名文件冲突
.PHONY: all clean run

