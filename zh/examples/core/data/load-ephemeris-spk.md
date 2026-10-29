# 读取 SPK 星历

该示例展示如何读取 SPICE SPK 二进制星历文件（`.bsp`），并获取星历时间区间、文件中的天体列表与天体位置速度。

::: tip
本示例依赖 CSPICE 动态库，首次调用时会自动加载该库；如果运行时报 `SPICE 库未加载`，请确认 cspice 库位于程序搜索路径（例如可执行文件所在目录），或通过启动配置中的 `CSPICE_LIBS` 指定库路径。[发布的二进制包](https://gitcode.com/space-ast/ast/releases)里有预编译的 cspice 库，可以直接使用。
:::

## 引入头文件

使用 `JplSpk` 类读取 SPK 星历数据文件，首先引入必要的头文件

同时设置本地编码为 UTF-8，避免程序在输出以 UTF-8 编码的中文时出现乱码。

```cpp title="load_ephemeris_spk.cpp"
#include "ast/JplSpk.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <clocale>
#include <string>
#include <vector>

// 设置本地编码为 UTF-8
const char* locale = std::setlocale(LC_ALL, ".UTF-8");

AST_USING_NAMESPACE
```

## 创建 JplSpk 实例并加载星历文件

调用 `open` 方法，输入 SPK 文件路径，返回错误码。

```cpp title="load_ephemeris_spk.cpp" indent
// 创建 JplSpk 实例并加载 SPK 星历文件（请替换为实际文件路径）
JplSpk spk;
errc_t err = spk.open("./data/kernels/spk/de430.bsp");
if (err != eNoError) {
    std::cerr << "无法加载SPK星历文件" << std::endl;
}
```

## 获取天体星历时间区间

调用 `getInterval` 方法，输入目标的 NAIF ID，返回该目标在文件中的可用时间区间。

```cpp title="load_ephemeris_spk.cpp" indent
// 获取地球（NAIF ID 399）的星历时间区间
TimeInterval interval;
err = spk.getInterval(399, interval);
if (err != eNoError) {
    std::cerr << "获取星历时间区间失败，错误码: " << err << std::endl;
} else {
    std::cout << "星历时间区间: " << interval.toString() << std::endl;
}
```

::: info
SPK 星历文件所使用的时间系统为 TDB （质心动力学时间），故其星历区间转换为 UTC 后一般不是整数时间点
例如：`1549-12-30 23:59:17.815889 UTC - 2650-01-24 23:58:50.815708 UTC`
:::

:::info

SPK文件使用NAIF ID标识天体，常用的ID与其对应的天体如下: 
- 0: 太阳系质心
- 1: 水星系质心
- 2: 金星系质心
- 3: 地月系质心
- 4: 火星系质心
- 5: 木星系质心
- 6: 土星系质心
- 7: 天王星系质心
- 8: 海王星系质心
- 9: 冥王星系质心
- 10: 太阳
- 199: 水星
- 299: 金星
- 399: 地球
- 301: 月球
- 499: 火星
- 599: 木星
- 699: 土星
- 799: 天王星
- 899: 海王星
- 999: 冥王星

其中 `xxx系质心` 表示考虑了行星的卫星（例如火卫一）后整个行星系统的质心。
个位数（1-9）通常代表行星系统的质心，而三位数（x99）则通常代表行星本体。
:::

## 获取星历文件中的天体列表

`getBodyIds()` 和 `getBodyNames()` 遍历 SPK 段描述符，返回该星历文件中的天体 ID 列表及其名称。

名称通过 CSPICE 的 `bodc2n` 转换为可读形式，未知 ID 会原样显示为 `NAIF ID <id>`。

```cpp title="load_ephemeris_spk.cpp" indent
// 列出文件中的天体
std::vector<std::string> names;
err = spk.getBodyNames(names);
if (err != eNoError) {
    std::cerr << "获取天体列表失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n该星历文件中的天体:" << std::endl;
    for (const auto& name : names) {
        std::cout << "    " << name << std::endl;
    }
}
```

## 查询天体位置速度

`getPosVelICRF()` 返回目标天体相对参考天体的位置和速度，参考系为以参考天体为中心的 ICRF（CSPICE 中的 `J2000`）。


```cpp title="load_ephemeris_spk.cpp" indent
Vector3d pos, vel;

// 查询 2000-01-01 12:00:00 UTC 时刻的天体状态
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);
// 地球相对地月系质心（NAIF ID 3）的位置和速度
err = JplSpk::getPosVelICRF(time, 399, 3, pos, vel);
if (err != eNoError) {
    std::cerr << "获取地球位置速度失败，错误码: " << err << std::endl;
} else {
    std::cout << "\n地球相对地月系质心的状态 (ICRF):" << std::endl;
    std::cout << "  位置 [m]:   " << toString(pos) << std::endl;
    std::cout << "  速度 [m/s]: " << toString(vel) << std::endl;
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
    std::cout << "\n月球相对地球的位置 [m]:" << toString(moonRelEarth) << std::endl;
    std::cout << "  地月距离: " << moonRelEarth.norm() << " m" << std::endl;
}
```

## 关闭星历文件

`JplSpk` 类在析构时会自动关闭已加载的星历文件，也可以调用 `close` 方法显式关闭已加载的星历文件。


```cpp title="load_ephemeris_spk.cpp" indent
spk.close();
```

完整代码下载：[load_ephemeris_spk.cpp](load_ephemeris_spk.cpp)
