# 时间

Time模块提供了全面的时间处理功能，包括日期、时间、日期时间、儒略日、简约儒略日、时长以及时间系统转换等功能。该模块是天体力学和航天动力学计算的基础组件，支持各种时间表示形式之间的转换和操作。

## 时间表示类型

模块定义了以下主要时间表示类型：

- `Date` (日期)
   - 表示公历日期，即年、月、日
   - 支持闰年判断、月份天数、年内天数、星期等功能

- `Time` (时间)
   - 表示一日内的时间，即时、分、秒
   - 支持时间规范化、总秒数计算等功能

- `DateTime` (日期时间)
   - 日期 + 时间，用于表示完整日期时间信息
   - 支持本地时间与UTC时间转换、日期时间规范化、格式化、解析等功能

- `JulianDate` (儒略日)
   - 用于天文学的时间表示方法
   - 支持日期时间与儒略日之间的转换

- `ModJulianDate` (简约儒略日)
   - 儒略日的简化形式
   - 基于JulianDate类，提供更简洁的表示方式

- `TimePoint`（绝对时间点）
    - 表示绝对时刻
    - 内部以 TAI 为基准存储，用于动力学计算

## 时间系统转换

模块支持以下时间系统之间的转换：

- TAI (原子时): 国际原子时
- TT (地球时): 地球动力学时，用于天体力学计算
- GPS (GPS时): 全球定位系统时间
- UTC (协调世界时): 世界协调时间


## 功能及入口

| 功能 | 头文件 | 
| --- | --- | 
| `Date` 类型 | `ast/Date.hpp` |  
| `Time` 类型 | `ast/Time.hpp` |  
| `DateTime` 类型 | `ast/DateTime.hpp` | 
| `JulianDate` 类型 | `ast/JulianDate.hpp` |
| `ModJulianDate` 类型 | `ast/ModJulianDate.hpp` | 
| `TimePoint` 类型 | `ast/TimePoint.hpp` | 
| 时间尺度转换函数 | `ast/TimeSystem.hpp` | 


## 注意事项

- `DateTime` 和 `JulianDate` 不携带时间尺度。同一个值可以按 UTC、TAI 或 TT 解释，尺度由转换函数赋予；`aDateTimeToJD` 只做历法字段与儒略日的换算，不做时间尺度转换。

- `TimePoint` 表示绝对时刻，内部以 TAI 为基准存储，可以转换为其他任何时间尺度下的儒略日或者日历时间。

- UTC ↔ TAI 转换依赖闰秒表，库内置了截至 2016 年底的闰秒数据

- TDB ↔ TT 的周期项计算基于解析公式，幅度约 1.6ms

- JulianDate 用 (int day, double second) 保证精度，避免单 double 的舍入误差

- 日期时间规范化函数有多个版本（本地时间、UTC时间、北京时间），请根据需要选择

## 相关

- [时间系统转换示例](../../examples/time-system-convert.md)

- [API 手册](https://space-ast.github.io/ast/api/group__Time.html)
