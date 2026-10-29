---
order: 1
---

# 时间体系

这一页解释 SpaceAST 为什么同时提供 `TimePoint`、`DateTime`、`JulianDate` 等多种时间类型，它们各自解决什么问题，以及时间尺度之间是什么关系。具体类型和函数索引见[时间](../reference/core/time.md)。

## 三种视角

同一个时刻，在程序里通常有三种用法：

- **给人看**：年、月、日、时、分、秒。对应 `Date`、`Time`、`DateTime`。
- **参与计算**：需要连续的、可加减的绝对时刻。对应 `TimePoint`。
- **与外部交换**：天文和航天领域的文件、接口常用儒略日。对应 `JulianDate`、`ModJulianDate`。

`DateTime` 和 `JulianDate` 都只是“数字和字段”，它们本身不携带时间尺度信息。同样一个 `DateTime`，既可以被解释成 UTC，也可以被解释成 TAI 或 TT。时间尺度是调用方赋予的语义，转换函数（例如 `aUTCToTAI`）负责把语义显式化。`TimePoint` 则是一个已经确定的绝对时刻，不依赖调用方再声明尺度。

## 时间尺度

| 尺度 | 含义 | 与 UTC 的关系 | 典型用途 |
| --- | --- | --- | --- |
| UTC | 协调世界时 | — | 日历、文件、用户输入 |
| TAI | 国际原子时 | UTC + 闰秒（当前 37 s） | 连续计时，不含闰秒 |
| TT | 地球时 | TAI + 32.184 s | 天体力学、轨道计算 |
| TDB | 质心动力学时 | TT + 周期项（约毫秒级） | 太阳系质心参考系 |
| GPS | GPS 时 | TAI − 19 s | GPS 相关数据 |
| UT1 | 世界时 1 | UTC + 地球自转修正（小于 0.9 s） | 地固系、地球自转相关计算 |

几个容易混淆的点：

- **UTC 不是均匀的**。闰秒会让 UTC 出现 23:59:60 这样的时刻，因此不能直接用 UTC 做长时间积分。
- **TT 和 TAI 相差固定值**，TT = TAI + 32.184 s。动力学计算通常使用 TT 或 TDB。
- **TDB 与 TT 的差是周期项**，量级在 1.7 ms 左右，高精度任务不能忽略。
- **UT1 与 UTC 的差不是常数**，由地球自转决定，必须读取 EOP 数据才能计算。没有 EOP 时，UT1 相关功能会退化或失败。
- **GPS 时与 TAI 相差固定值**，GPS = TAI − 19 s。

库在 UTC ↔ TAI/TT 的转换中内置了闰秒数据，调用方不需要自己加 37 秒；但 UT1 依赖外部 EOP 数据，需要先初始化运行时数据。

## 精度问题

儒略日的数值量级约为 \(2.45\times10^6\)。在这个量级上，一个 `double` 的最小间隔约为几十微秒：

\[
2^{21-52} \text{ day} \approx 4.7\times10^{-10}\ \text{day} \approx 40\ \mu s
\]

这意味着用单个 `double` 存儒略日，无法稳定表示微秒级的时刻。`JulianDate` 因此把时刻拆成“整数日 + 当日秒”两部分，`TimePoint` 则使用“`int64` 整数秒 + `double` 小数秒”。需要高精度时：

- 用 `JulianDate` 或 `TimePoint`，不要用 `ImpreciseJD` 做中间计算；
- 时间间隔优先用 `LongDuration` 或 `DaySecDuration`，不要反复累加 `double` 秒数。

`TimePoint` 的内部参考是 **J2000 TT**，不是 Unix 纪元，也不是儒略日的整数日。它的整数部分和小数部分应作为一个整体使用，不要单独解释。

## 如何选择

| 场景 | 推荐类型 |
| --- | --- |
| 轨道传播、动力学积分、时刻加减 | `TimePoint` |
| 读取/写入需要儒略日的文件或接口 | `JulianDate`；只需日数时用 `ModJulianDate` |
| 日历显示、用户输入、日志 | `DateTime` |
| 短暂的时间间隔（秒级） | `double` 或 `ShortDuration` |
| 长时间跨度、需要保留精度 | `LongDuration` 或 `DaySecDuration` |
| 明确指定时间尺度进行计算 | `TimePoint`，或 `DateTime` + 显式转换函数 |

实践中的常见组合是：入口把用户输入解析成 `DateTime`（UTC），转成 `TimePoint` 参与计算，输出时再转回 `DateTime`；需要和外部星历、STK 等工具交换时，通过 `JulianDate` 转换。

## 相关文档

- [时间](../reference/core/time.md)：类型、函数和常量索引
- [时间系统转换示例](../examples/time-system-convert.md)：UTC/TAI/TT/TDB/UT1 的实际转换代码
- [单位与约定](../guide/units-and-conventions.md)：时间单位和默认约定
