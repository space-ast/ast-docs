---
order: 4
---

<!-- 本文档已由维护者人工校对 -->

# 数据与运行时

本项目的二进制发布包默认不携带星历、EOP、重力场等数据。而涉及时间系统、地固系、星历或高精度力模型的功能，需要数据文件才能正常工作。

该文档将说明需要哪些数据、库按什么顺序查找、怎样指定路径，以及部署程序时的注意事项。

## 数据文件

| 数据文件 | 默认相对路径 | 用途 |
| --- | --- | --- |
| EOP | `SolarSystem/Earth/EOP-All.txt` | UT1、极移、地固系变换 |
| JPL DE 星历 | `SolarSystem/plneph.430` | 行星位置、月球姿态、三体引力 |
| IAU 岁差章动系数 | `IERS-conventions/2010/tab5.2a.txt` 等 | 高精度坐标变换 |
| IAU-XYS 预计算数据 | `IERS-conventions/2010/IAU2006_XYS.dat` | 加速岁差章动计算 |
| 太阳系目录 | `SolarSystem/` | 天体参数、重力场文件（`.grv`、`.cof`） |
| SPK 星历 | `kernels/spk/*.bsp` | 行星和小天体星历 |
| 闰秒 | `Time/Leap_Second.dat` | UTC↔TAI/TT 转换 |

对应关系大致是：

- 纯数学、二体传播、轨道根数换算：**不需要数据**。
- UTC/TAI/TT 互转：使用内置闰秒数据，通常不需要额外文件；只有显式指定时才从文件加载。
- UT1、ECF 坐标、地面站相关计算：需要 **EOP**。
- 行星、太阳、月球位置，以及依赖它们的力模型：需要 **JPL DE** 或 **SPK**。
- HPOP 的高阶重力场：需要 `SolarSystem/` 下对应重力场系数文件，例如 `Earth/EGM2008.grv`。

## 默认查找顺序

没有显式设置时，`aDataDirGet()` 按以下顺序确定数据目录：

1. 环境变量 `AST_DATA_DIR` 指向的目录；
2. 动态库所在目录下的 `data/`；
3. 可执行文件所在目录下的 `data/`；
4. 当前工作目录下的 `data/`。

程序里可以随时打印实际使用的目录：

```cpp
#include "ast/RunTime.hpp"
#include <iostream>

int main()
{
    errc_t rc = aInitialize();
    std::cout << "数据目录: " << aDataDirGet() << std::endl;
    if (rc != eNoError) {
        std::cerr << "初始化返回错误码: " << rc << std::endl;
    }
    return 0;
}
```

也可以手动指定一个已存在的数据目录，然后调用 `aInitialize()` 加载其中的数据

```cpp
aDataDirSet("D:/ast-data");
aInitialize();
```


## 初始化流程

`aInitialize()` 会：

1. 确定数据目录；
2. 如果目录为空或不存在，尝试**自动下载**数据（见下一节）；
3. 加载 IAU 岁差章动系数；
4. 加载 JPL DE、EOP、太阳系目录和默认 SPK 星历。

其中任何一项失败，`aInitialize()` 都会返回非零错误码，并输出“初始化失败，有部分数据文件加载失败”之类的日志。

初始化流程不会因为某个文件缺失而崩溃，但后续依赖该数据的功能可能失败或精度下降。

只使用二体、轨道根数换算这类纯计算时，可以不调用 `aInitialize()`。

## 启动配置文件

如果你的程序需要把数据放在固定位置，推荐使用启动配置文件。`aInitialize()` 会自动在以下位置查找 `ast_startup_file.txt`：

1. 动态库所在目录；
2. 可执行文件所在目录。

文件格式是键值对，支持用已有键拼接路径：

```txt
DATA_DIR        = ./data/
SOLARSYSTEM_DIR = DATA_DIR/SolarSystem/
JPLDE_FILE      = DATA_DIR/SolarSystem/plneph.430
EOP_FILE        = DATA_DIR/SolarSystem/Earth/EOP-All.txt
SPK_FILES       = DATA_DIR/kernels/spk/planets.bsp
SPK_FILES      += DATA_DIR/kernels/spk/jupiter.bsp
```

配置文件里的相对路径按**配置文件所在目录**进行解析。

常用键：

| 键 | 含义 |
| --- | --- |
| `DATA_DIR` | 数据根目录 |
| `SOLARSYSTEM_DIR` | 太阳系数据目录 |
| `JPLDE_FILE` | JPL DE 星历文件 |
| `EOP_FILE` | EOP 文件 |
| `LSK_FILE` | 闰秒文件（可选） |
| `IAUX_FILE` / `IAUY_FILE` / `IAUS_FILE` | IAU 岁差章动系数 |
| `IAUXYS_PRECOMPUTED_FILE` | 预计算的 XYS 数据 |
| `SPK_FILES` | SPK 文件列表，重复写 `+=` 追加 |

也可以直接指定配置文件，而不依赖自动查找：

```cpp
aInitializeByConfig("D:/my_project/ast_startup_file.txt");
```

## 自动下载与离线使用

数据目录为空时，`aInitialize()` 会尝试下载数据。

这对第一次运行很方便，但在离线环境会失败。此时需要[手动下载](https://gitcode.com/space-ast/ast-data/tags)，并将数据放到指定目录，也可以通过设置 `AST_DATA_DIR`环境变量指定目录，或在启动配置文件里指定 `DATA_DIR`。


## 运行时库部署

除了数据文件，程序运行时还需要能找到动态库：

- **Windows**：将用到的 DLL 拷贝到 `.exe` 同目录，其中Debug 版库名带 `_d` 后缀（如 `AstCore_d.dll`）。

- **Linux**：设置 `LD_LIBRARY_PATH`，或给可执行文件设置 rpath。

根据所使用的功能按需链接各个模块，但在 linux 上链接时要把依赖链上的下层模块都加上。

