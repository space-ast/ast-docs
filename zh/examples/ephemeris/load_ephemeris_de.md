# 读取 JPL DE 星历

该示例展示如何用 `JplDe` 读取 JPL DE 二进制星历（DE440、DE441 等），并获取星历时间区间与天体位置速度。


## 引入头文件

使用 `JplDe` 类读取 JPL DE 系列星历数据文件。

```cpp title="load_ephemeris_de.cpp"
#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE
```

## 创建 JplDe 实例并加载星历文件

`open()` 接收星历文件路径并返回错误码

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
std::cout << "星历时间范围: " << std::endl;
std::cout << "  开始时间: " << interval.getStart().toString() << std::endl;
std::cout << "  结束时间: " << interval.getStop().toString() << std::endl;
```

## 查询天体位置速度

`getPosVelICRF()` 返回目标天体相对参考天体的位置和速度，参考系为以参考天体为中心的 ICRF，单位为 m 和 m/s。

```cpp title="load_ephemeris_de.cpp" indent
// 查询 2000-01-01 12:00:00 UTC 时刻的天体状态
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);

Vector3d pos, vel;

// 地球相对太阳系质心的位置和速度
err = de.getPosVelICRF(time, JplDe::eEarth, JplDe::eSSBarycenter, pos, vel);
if (err != eNoError) {
    std::cerr << "获取地球位置速度失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n地球相对太阳系质心的状态 (ICRF):" << std::endl;
    std::cout << "  位置 [m]:   " << std::setprecision(6)
              << pos.x() << "  " << pos.y() << "  " << pos.z() << std::endl;
    std::cout << "  速度 [m/s]: " << std::setprecision(6)
              << vel.x() << "  " << vel.y() << "  " << vel.z() << std::endl;
}
```

## 计算地月距离

```cpp title="load_ephemeris_de.cpp" indent
// 月球相对地球的位置，并计算地月距离
err = de.getPosVelICRF(time, JplDe::eMoon, JplDe::eEarth, pos, vel);
if (err != eNoError) {
    std::cerr << "获取月球位置速度失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n月球相对地球的位置 [m]:" << std::endl;
    std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
    std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
    std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;
    std::cout << "  地月距离: " << std::setprecision(6) << pos.norm() << " m" << std::endl;
}
```

## 关闭星历文件

```cpp title="load_ephemeris_de.cpp" indent
de.close();
```

完整代码下载：[load_ephemeris_de.cpp](load_ephemeris_de.cpp)
