---
order: 3
---

<!-- 本文档已由维护者人工校对 -->

# 单位与约定

该文档将介绍本工程的单位约定，以及所采用的时间系统约定、坐标系统约定，避免在使用本工程时产生数值错误。

## 函数接口的数值单位

除个别接口在注释中特别说明外，长度、速度、角度、时间等都使用国际单位制：

| 物理量 | 单位 | 示例 |
| --- | --- | --- |
| 长度 | 米（m） | 位置、半径、半长轴、高度 |
| 速度 | 米每秒（m/s） | 速度、相对速度 |
| 角度 | 弧度（rad） | 轨道根数、姿态角、经纬度 |
| 角速度 | 弧度每秒（rad/s） | 平均角速度、自转角速度 |
| 时间 | 秒（s） | 传播时长、时间间隔 |
| 引力常数 | 立方米每二次方秒（m³/s²） | 中心天体 GM |
| 质量 | 千克（kg） | 航天器质量 |

::: info
时间点本身用 `TimePoint` 表示，不转换成秒；只有**时间间隔**才用秒。
:::

### 单位字面量

`ast/Literals.hpp` 提供了一组单位字面量，可在代码里直接写带单位的数值，并在C++编译期自动换算成国际单位制对应的值。

使用时需要引入 `ast::literals` 命名空间

```cpp
using namespace ast::literals;

double a = 7000_km;        // 7.0e6 m
double b = 7.5_km_s;       // 7500 m/s
double c = 90.0_deg;       // pi/2 rad
double d = 24.0_h;         // 86400 s
double e = 1.0_au;         // 天文单位，将被换算为米
```

常用单位：

| 类别 | 可用字面量 |
| --- | --- |
| 角度 | `_rad`、`_deg`、`_arcsec`、`_revs` |
| 长度 | `_m`、`_km`、`_cm`、`_mm`、`_um`、`_nm`、`_au` |
| 时间 | `_s`、`_min`、`_h`、`_hour`、`_day`、`_ms`、`_us`、`_ns`、`_ps` |
| 速度 | `_m_s`、`_km_s`、`_km_h` |
| 角速度 | `_rad_s`、`_deg_s` |
| 质量 | `_kg`、`_g` |
| 面积 | `_m2`、`_km2` |

::: warning

字面量只做**数值换算**，返回的仍是 `double`，不携带量纲。

例如 `2_km * 3_km` 得到的是数值 `6e6`，但类型上只是普通 `double`，不会做量纲检查。

如果需要严格的量纲运算与检查，请使用[单位量纲系统](../reference/util/quantity.md)。
:::

## 物理常量

`ast/Constants.hpp` 中定义了常用的物理常量：

| 常量 | 含义 | 值 |
| --- | --- | --- |
| `kPI` / `kTwoPI` | 圆周率 | 3.14159... / 6.28318... |
| `kDegToRad` / `kRadToDeg` | 角度与弧度换算 | 0.0174533 / 57.2958 |
| `kAU` | 天文单位 | 1.49597870691e11 m |
| `kLightSpeed` | 光速 | 2.99792458e8 m/s |
| `kEarthGrav` | 地球引力常数（WGS84） | 3.986004418e14 m³/s² |
| `kEarthRadius` | 地球赤道半径（WGS84） | 6378137.0 m |
| `kEarthFlatFact` | 地球扁率（WGS84） | 3.35281066475e-3 |
| `kEarthSiderealDay` | 地球平恒星日 | 86164.09054 s |
| `kEarthAngVel` | 地球自转角速度 | 7.292115146706979e-5 rad/s |

不同模型给出的 GM 略有差异（例如 `kEarthGrav_EGM2008`），做高精度任务分析时要注意和所选重力场模型保持一致。

## 时间约定

`TimePoint` 表示一个**绝对时刻**，适合参与动力学计算。

`JulianDate` 主要用在需要儒略日的接口中。

`DateTime` 表示日历时间，适用用于打印输出。

```cpp
#include "ast/TimePoint.hpp"
#include "ast/JulianDate.hpp"
#include "ast/TimeSystem.hpp"

AST_USING_NAMESPACE

int main(){
    TimePoint tp = TimePoint::FromUTC(2026, 1, 1, 0, 0, 0);

    double jdUTC, jdTAI, jdTT, jdUT1;
    aTimePointToUTC(tp, jdUTC);
    aTimePointToTAI(tp, jdTAI);
    aTimePointToTT (tp, jdTT);
    aTimePointToUT1(tp, jdUT1);   // 需要 EOP 数据
}
```

::: tip

- 表示绝对时间点应当使用 `TimePoint`，因为使用 `JulianDate` 和 `DateTime` 时会存在时间系统上的歧义（可能是 UTC，也可能是 TAI、TT、TDB 等），且在时间递进时需要考虑闰秒(2017年前)

- 表示儒略日时，不要使用单个 `double` ，其数值精度不够，应当使用 `JulianDate` 表示

:::

## 坐标系约定


- ICRF、J2000：惯性系，用于进行轨道动力学计算

- ECEF：地固系，随地球自转，用于经纬度、地面站、地形相关计算

在本工程的实现里，ICRF 和 ECF 之间的转换采用 IAU 2006 规范，J2000 与 ECF 之间的转换采用 IAU 1980 规范。


```cpp
#include "ast/FrameTransform.hpp"
#include "ast/TimePoint.hpp"
#include "ast/Vector.hpp"
#include "ast/Literals.hpp"

AST_USING_NAMESPACE
using namespace literals;

int main(){
    TimePoint tp = TimePoint::FromUTC(2026, 1, 1, 0, 0, 0);
    Vector3d rJ2000{7000_km, 0.0, 0.0};
    Vector3d rECF;

    aJ2000ToECF(tp, rJ2000, rECF);
}
```

