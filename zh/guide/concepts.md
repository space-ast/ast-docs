---
order: 2
---

<!-- 本文档已由维护者人工校对 -->

# 核心概念

该文档将介绍：本工程的模块组成、代码的命名约定、错误处理、对象生命周期、初始化等，为使用本工程的用户提供必要的背景知识。

## 模块与依赖

本工程按功能拆成多个模块，每个模块将对应一个动态库和一组头文件，您可按需链接这些模块。

核心模块：

| 模块 | 提供的内容 | 常见头文件 |
| --- | --- | --- |
| AstUtil | 日志、常量、单位字面量、文件系统、字符串、智能指针等基础设施 | `ast/Logger.hpp`、`ast/Constants.hpp`、`ast/Literals.hpp` |
| AstMath | 向量与矩阵、四元数/欧拉角、ODE 积分、插值、非线性方程 | `ast/Vector.hpp`、`ast/Matrix.hpp`、`ast/Quaternion.hpp` |
| AstWeather | 大气、地磁、辐射带模型 | `ast/Atmosphere.hpp` |
| AstCore | 时间系统、坐标系、轨道根数、预报器、星历、力模型 | `ast/TimePoint.hpp`、`ast/OrbitElement.hpp`、`ast/HPOP.hpp` |
| AstSPICE | 与 NASA SPICE 兼容的接口 | `ast/SpiceZpr.hpp` |
| AstScript | 内嵌脚本解释器 | `ast/ScriptAPI.hpp` |
| AstSim | 航天器、地面站等仿真对象 | `ast/Spacecraft.hpp` |


例如程序只做时间换算，则只需要链接 `AstCore` 模块，但在 linux 上链接时要把依赖链上的下层模块都加上。

头文件可以统一通过 `ast/` 前缀引入

```cpp
#include "ast/TimePoint.hpp"
#include "ast/HPOP.hpp"
```

也可以按模块名称前缀引入

```cpp
#include "AstCore/TimePoint.hpp"
#include "AstCore/HPOP.hpp"
```

::: info
上面两种写法都是合法的，但推荐统一用 `ast/` 前缀，因为您不需要记住功能所在的模块名称，可以减少心智负担。
:::

## 命名约定

本工程的所有公共函数和类型都默认在 `ast` 命名空间下，并遵循固定前缀：

| 前缀 | 含义 | 示例 |
| --- | --- | --- |
| `a` | 全局函数 | `aInitialize()`、`aTwoBodyProp()` |
| `E` / `e` | 枚举类型 / 枚举值 | `ETimeScale { eUTC, eTAI }` |
| `k` | 常量 | `kPI`、`kDegToRad` |
| `AST_`、`A_` | 宏 | `AST_USING_NAMESPACE`、`A_CXX_VERSION` |

在源文件里用 `AST_USING_NAMESPACE` 引入命名空间，它默认展开为 `using namespace ast;`

```cpp
#include "ast/TimePoint.hpp"

AST_USING_NAMESPACE

int main()
{
    TimePoint tp = TimePoint::FromUTC(2026, 1, 1, 0, 0, 0);
    return 0;
}
```

::: warning
如果您在自己的代码里定义了与本工程同名的类型或函数，为避免命名冲突，请显式写 `ast::` 限定，同时避免在全局范围内使用 `AST_USING_NAMESPACE`
例如
```cpp
#include "ast/TimePoint.hpp"

int main()
{
    ast::TimePoint tp = ast::TimePoint::FromUTC(2026, 1, 1, 0, 0, 0);
    return 0;
}
```
:::

## 错误处理

库的接口以**错误码**为主，不依赖 C++ 异常表达错误。

函数的返回值类型是 `errc_t`（其定义为`int`的别名）。

值为 `0` (`eNoError`)表示成功，其余取值表示错误：

```cpp{16,17}
#include "ast/TwoBody.hpp"
#include "ast/Vector.hpp"
#include "ast/Constants.hpp"
#include "ast/Literals.hpp"
#include "ast/Logger.hpp"

AST_USING_NAMESPACE
using namespace literals;

int main()
{
    Vector3d r{7000_km, 0.0, 0.0};
    Vector3d v{0.0, 7.5_km_s, 0.0};

    errc_t rc = aTwoBodyProp(3600.0, kEarthGrav, r, v);
    if (rc != eNoError) {
        aError("二体预报失败，错误码：%d", rc);
        return 1;
    }
    return 0;
}
```

::: tip
- 对会读文件、访问网络或依赖数据的接口，**应当始终检查返回值**；
- `aDebug` / `aInfo` / `aWarning` / `aError` 是库的日志宏，用 printf 风格的格式串。它们输出诊断信息，不改变控制流。
:::

## 对象与内存

本工程里的类型大致分两类：

- **值类型**：`TimePoint`、`JulianDate`、`DateTime`、`Vector3d`、`Matrix3d` 等。直接按值创建、拷贝和返回，生命周期由其所在作用域决定，不需要手动释放。

- **对象类型**：坐标系、星历点、航天器、天体等继承自 `Object` 的类型，其使用引用计数管理生命周期，需要配合库提供的智能指针使用：
  - `SharedPtr<T>`：共享所有权，最后一个引用释放时销毁对象，类似 `std::shared_ptr` ；
  - `WeakPtr<T>`：不增加引用计数，用于观察已被 `SharedPtr` 管理的对象，类似 `std::weak_ptr` 。

::: info
智能指针只能用于 `Object` 派生类。普通值类型不要套 `SharedPtr`。详细规则见[智能指针](../reference/util/smart-pointer.md)。
:::

## 初始化

涉及地球指向、星历、重力场或大气模型时，需要初始化所需的运行时数据。

一般情况下，本工程会在相关功能被使用时自动初始化运行时数据，您也可以手动调用 `aInitialize()` 显式初始化：

```cpp
#include "ast/RunTime.hpp"

int main()
{
    errc_t rc = aInitialize();
    if (rc != eNoError) {
        // 部分数据文件加载失败，后续依赖这些数据的功能可能不可用或者功能回退到简化版
        return 1;
    }

    // ... 计算 ...

    aUninitialize();
    return 0;
}
```

只做纯数学运算（向量、矩阵、二体传播、轨道根数换算）时不必调用 `aInitialize()`。

关于初始化具体加载哪些数据、数据的位置，请参考[数据与运行时](data-and-runtime.md)。

