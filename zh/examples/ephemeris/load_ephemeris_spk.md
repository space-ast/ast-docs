# 读取 SPK 星历

该示例展示如何用 `JplSpk` 读取 SPICE SPK 二进制星历文件（`.bsp`），并获取星历时间区间、文件中的天体列表与天体位置速度。

::: tip
本示例依赖 CSPICE 动态库。首次调用时会自动加载该库；如果运行时报“SPICE 库未加载”，请检查构建输出目录中是否存在 cspice 库，或通过 `CSPICE_LIBS` 配置指定库路径。
:::

## 引入头文件

使用 `JplSpk` 类读取 SPK 星历数据文件。

```cpp title="load_ephemeris_spk.cpp"
#include "ast/JplSpk.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

AST_USING_NAMESPACE
```

## 创建 JplSpk 实例并加载星历文件

`open()` 接收 SPK 文件路径并返回错误码。同一路径重复打开时会先关闭旧文件，`close()` 可由析构函数自动调用。

```cpp title="load_ephemeris_spk.cpp" indent

// 创建 JplSpk 实例并加载 SPK 星历文件（请替换为实际文件路径）
JplSpk spk;
errc_t err = spk.open("./data/kernels/spk/de430.bsp");
if (err != eNoError) {
    std::cerr << "无法加载SPK星历文件" << std::endl;
    return -1;
}
```

## 获取星历时间区间

`getInterval()` 的第一个参数是目标的 NAIF ID，返回该目标在文件中覆盖的时间区间。第一次调用会解析文件并缓存结果。

```cpp title="load_ephemeris_spk.cpp" indent
// 获取地球（NAIF ID 399）的星历时间区间
TimeInterval interval;
err = spk.getInterval(399, interval);
if (err != eNoError) {
    std::cerr << "获取星历时间区间失败，错误码: " << err << std::endl;
} else {
    std::cout << "星历时间范围: " << std::endl;
    std::cout << "  开始时间: " << interval.getStart().toString() << std::endl;
    std::cout << "  结束时间: " << interval.getStop().toString() << std::endl;
}
```

## 列出文件中的天体

`getBodyIds()` 和 `getBodyNames()` 遍历 SPK 段描述符，返回文件中出现过的目标体及其名称。名称通过 CSPICE 的 `bodc2n` 转换为可读形式，未知 ID 会原样显示为 `NAIF ID <id>`。

```cpp title="load_ephemeris_spk.cpp" indent
// 列出文件中的天体
std::vector<std::string> names;
err = spk.getBodyNames(names);
if (err != eNoError) {
    std::cerr << "获取天体列表失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n文件中的天体:" << std::endl;
    for (const auto& name : names) {
        std::cout << "  " << name << std::endl;
    }
}
```

## 查询天体位置速度

`getPosVelICRF()` 返回目标天体相对参考天体的位置和速度，参考系为以参考天体为中心的 ICRF（CSPICE 中的 `J2000`），单位为 m 和 m/s。


```cpp title="load_ephemeris_spk.cpp" indent
// 查询 2000-01-01 12:00:00 UTC 时刻的天体状态
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);

Vector3d pos, vel;

// 地球相对地月系质心（NAIF ID 3）的位置和速度
err = JplSpk::getPosVelICRF(time, 399, 3, pos, vel);
if (err != eNoError) {
    std::cerr << "获取地球位置速度失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n地球相对地月系质心的状态 (ICRF):" << std::endl;
    std::cout << "  位置 [m]:   " << std::setprecision(6)
              << pos.x() << "  " << pos.y() << "  " << pos.z() << std::endl;
    std::cout << "  速度 [m/s]: " << std::setprecision(6)
              << vel.x() << "  " << vel.y() << "  " << vel.z() << std::endl;
}
```

## 计算地月距离

地球和月球在同一文件中的参考天体都是地月系质心，取两者位置之差即为月球相对地球的位置。

```cpp title="load_ephemeris_spk.cpp" indent
// 月球相对地月系质心的位置，与地球位置作差得到地月相对位置
Vector3d earthPos, earthVel;
Vector3d moonPos, moonVel;

errc_t errEarth = JplSpk::getPosVelICRF(time, 399, 3, earthPos, earthVel);
errc_t errMoon = JplSpk::getPosVelICRF(time, 301, 3, moonPos, moonVel);
if (errEarth != eNoError || errMoon != eNoError) {
    std::cerr << "获取地月位置速度失败，错误码: " << errEarth << ", " << errMoon << std::endl;
} else {
    Vector3d moonRelEarth = moonPos - earthPos;
    std::cout << "\n月球相对地球的位置 [m]:" << std::endl;
    std::cout << "  X: " << std::setprecision(6) << moonRelEarth.x() << std::endl;
    std::cout << "  Y: " << std::setprecision(6) << moonRelEarth.y() << std::endl;
    std::cout << "  Z: " << std::setprecision(6) << moonRelEarth.z() << std::endl;
    std::cout << "  地月距离: " << std::setprecision(6) << moonRelEarth.norm() << " m" << std::endl;
}
```

## 关闭星历文件

```cpp title="load_ephemeris_spk.cpp" indent
spk.close();
```

完整代码下载：[load_ephemeris_spk.cpp](load_ephemeris_spk.cpp)
