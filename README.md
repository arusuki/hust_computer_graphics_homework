# ALGO: 华中科技大学计算机图形学作业（独孤信印章）

理论上本项目可以在多平台上构建。经过测试的平台有windows+VS2022，windows+mingw8.1.0，macos+xcode14.0。注意依赖库assimp在VS2022下**Debug配置**编译时会缺少pdb文件，暂时没有解决方案，请使用**Release配置**进行构建。

提供windows下的预编译二进制文件。

## 1. 安装glfw

- 从源码构建

​	进入glfw源码目录，使用以下命令进行安装。

```
cmake -B build -G <your_generator>
cmake --build build
cmake install build --prefix=<path/to/your/glfw/prefix>
```

- 使用包管理工具进行安装

​	Macos下，使用homebrew安装glfw。

```
brew install glfw
```

​	Linux下可用自带包管理器安装（以apt为例），[可以参考这个文档](https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Linux)。

```
sudo apt-get install build-essential

sudo apt-get install build-essential libgl1-mesa-dev

sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev

sudo apt-get install libglfw3-dev libglfw3
```

## 2. 项目构建

​	使用cmake进行项目构建和安装，如果使用源码安装的方式安装的glfw，需要在config步骤中告诉cmake实际的glfw的安装位置（`-D CMAKE_PREFIX_PATH=<path/to/your/glfw/prefix> `）。为确保ABI兼容，这里选择生成器最好与构建glfw的生成器相同。

```
cmake -B build -G <your_generator> [-D CMAKE_PREFIX_PATH=<path/to/your/glfw/prefix>]

cmake --build build [--config=Release]

cmake --install build --prefix=<path/to/where/you/want/to/install/algo> [--config=Release]
```

## 3. 项目运行

​	可执行文件位于上述构建路径中的	`<path/to/where/you/want/to/install/algo/bin>`下，直接运行即可。

​	项目渲染两个印章，包含一个点光源和跟随视角移动的手电筒。按键WASD可在场景中移动，U和J可以控制模型外接球半径，H和K可以控制小模型旋转速度。视角可随鼠标移动转动。本项目实现了带衰减的phone shading和基于法线贴图的凹凸纹理。运行截图如下。

![example1](./docs/example1.png)

![示例](./docs/example2.png)