# 读取 JPL DE 星历

该示例展示如何读取 JPL DE 二进制星历（DE440、DE441 等），并获取星历时间区间，以及查询天体的相对位置速度。


## 引入头文件

将使用 `JplDe` 类读取 JPL DE 系列星历数据文件，首先引入必要的头文件。

同时设置本地编码为 UTF-8，避免程序在输出以 UTF-8 编码的中文时出现乱码。

```cpp title="load_ephemeris_de.cpp"
#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>     // for std::cout, std::cerr, std::endl
#include <clocale>      // for std::setlocale

const char* locale = std::setlocale(LC_ALL, ".UTF-8");

AST_USING_NAMESPACE     // 引入 SpaceAST 的命名空间

```

## 创建 JplDe 实例并加载星历文件

调用 `open()` 方法，输入星历文件路径，并返回错误码

```cpp title="load_ephemeris_de.cpp" indent

// 创建 JplDe 实例并加载 JPL DE 星历文件（请替换为实际文件路径）
JplDe de;
errc_t err = de.open("./data/SolarSystem/plneph.430");
if (err != eNoError) {
    std::cerr << "无法加载DE星历文件" << std::endl;
    return -1;
}
```

## 获取星历时间区间

```cpp title="load_ephemeris_de.cpp" indent
// 获取星历时间区间
TimeInterval interval;
de.getInterval(interval);
std::cout << "星历时间区间: " << interval.toString() << std::endl;
```

::: info
DE 星历所使用的时间系统为 TDB （质心动力学时间），故其星历区间转换为 UTC 后一般不是整数时间点
例如：`1949-12-13 23:59:17.816568 UTC - 2150-01-21 23:58:50.815541 UTC`
:::

## 查询天体位置速度

`JplDe::getPosVelICRF` 可计算目标天体相对参考天体的位置和速度，参考系为以参考天体为中心的 ICRF。

所支持的天体和其相应的枚举值如下表：

| 天体 | 枚举值 | 
|------|--------|
| 水星系质心 | `JplDe::eMercury` | 
| 金星系质心 | `JplDe::eVenus` |
| 地球 | `JplDe::eEarth` | 
| 火星系质心 | `JplDe::eMars` | 
| 木星系质心 | `JplDe::eJupiter` | 
| 土星系质心 | `JplDe::eSaturn` | 
| 天王星系质心 | `JplDe::eUranus` | 
| 海王星系质心 | `JplDe::eNeptune` | 
| 冥王星系质心 | `JplDe::ePluto` |
| 月球 | `JplDe::eMoon` | 
| 太阳 | `JplDe::eSun` | 
| 太阳系质心 | `JplDe::eSSBarycenter` | 
| 地月系质心 | `JplDe::eEMBarycenter` | 

以下为计算地球(`JplDe::eEarth`)相对太阳系质心(`JplDe::eSSBarycenter`)的位置和速度的示例：

```cpp title="load_ephemeris_de.cpp" indent
Vector3d pos, vel;

// 时刻: 2000-01-01 12:00:00 UTC 
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);
// 计算地球相对太阳系质心的位置和速度
err = de.getPosVelICRF(time, JplDe::eEarth, JplDe::eSSBarycenter, pos, vel);
if (err != eNoError) {
    std::cerr << "获取地球位置速度失败" << std::endl;
} else {
    std::cout << "\n地球相对太阳系质心的状态 (ICRF):" << std::endl;
    std::cout << "  位置 [m]:   " << toString(pos) << std::endl;
    std::cout << "  速度 [m/s]: " << toString(vel) << std::endl;
}
```



## 计算地月距离


计算月球(`JplDe::eMoon`)相对地球(`JplDe::eEarth`)的距离：

```cpp title="load_ephemeris_de.cpp" indent
// 计算月球相对地球的位置，并计算地月距离
err = de.getPosVelICRF(time, JplDe::eMoon, JplDe::eEarth, pos, vel);
if (err != eNoError) {
    std::cerr << "获取月球位置速度失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n月球相对地球的位置 [m]:" << toString(pos) << std::endl;
    std::cout << "  地月距离: " << pos.norm() << " m" << std::endl;
}
```

## 关闭星历文件

`JplDe` 类在析构时会自动关闭已加载的星历文件，也可以调用 `close` 方法显式关闭已加载的星历文件

```cpp title="load_ephemeris_de.cpp" indent
de.close();
```

完整代码下载：[load_ephemeris_de.cpp](load_ephemeris_de.cpp)
