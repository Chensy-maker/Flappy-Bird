# Flappy Bird - 纯C语言图形版
基于 SDL2 图形库实现的 Flappy Bird 2D 小游戏，采用精灵贴图渲染和有限状态机架构。
## 特性
- 🎮 纯C语言开发，跨平台支持
- 🖼️ PNG 精灵贴图完整渲染
- 🔢 数字贴图拼接显示分数（无字体依赖）
- 🎵 OGG 音效支持
- 💾 最高分持久化存储
- 🎯 有限状态机流程管理
## 环境要求
### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
其他Linux发行版
请安装对应的 SDL2、SDL2_image、SDL2_mixer 开发包。

编译运行
<BASH>
# 创建构建目录
mkdir build && cd build
# 配置编译
cmake ..
# 编译
make
# 运行
./flappy_bird
操作说明
按键	功能
空格	开始游戏 / 小鸟跳跃 / 重新开始
ESC	退出游戏
项目结构
<TEXT>
Flappy-Bird/
├── assets/          # 游戏素材资源
│   ├── images/      # PNG 图片素材（背景、角色、数字等）
│   └── sounds/      # OGG 音效文件
├── data/            # 最高分持久化存储
├── include/         # 头文件
├── src/             # 源代码
│   ├── main.c       # 程序入口
│   ├── game.c       # 游戏逻辑核心
│   ├── render.c     # 渲染引擎
│   ├── input.c      # 输入处理
│   └── physics.c    # 物理模拟
└── CMakeLists.txt   # 构建配置
技术实现
1. 精灵贴图渲染
使用 SDL2_image 加载 PNG 图片，通过纹理映射实现游戏场景、角色、UI 元素的完整渲染。

2. 数字贴图拼接
无字体文件依赖，使用 0-9 独立数字 PNG 图片，通过逐位拆分分数并偏移贴图实现分数显示，避免字体缺失或乱码问题。

3. 有限状态机架构
<TEXT>
菜单状态 → 游戏运行状态 → 游戏结束状态
   ↑_______________|_______________↓
分层管理输入、物理、渲染逻辑，结构清晰易于维护和扩展。

4. 碰撞检测
基于 AABB（轴对齐边界框）算法实现小鸟与管道、地面的精确碰撞检测。

5. 最高分持久化
使用文件系统存储最高分记录，程序重启后自动加载。

素材来源
游戏图片、场景、角色、数字素材来源：CSDN Blog

版本管理
项目通过 GitHub 实现多设备同步开发，空目录使用 .gitkeep 占位文件确保目录结构完整提交。

开发说明
代码遵循 C99 标准
使用 CMake 跨平台构建
资源路径相对于可执行文件目录
建议帧率：60 FPS
