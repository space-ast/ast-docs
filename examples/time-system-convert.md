# 时间系统转换

## 概述

航天任务中常用六种时间系统：UTC、TAI、TT、TDB、UT1 和 GPS。它们之间的关系由常数偏移、闰秒累计和周期项决定：

```
TAI = UTC + ΔAT     (闰秒累计, 2024年为37s)
TT  = TAI + 32.184s (常数偏移)
TDB = TT  + P(t)    (周期项, 幅度约1.6ms)
GPS = TAI - 19s     (常数偏移)
UT1 = UTC + DUT1    (地球自转修正, |DUT1|<0.9s)
```

本例演示 TimePoint、JulianDate、DateTime 三种时间表示形式之间的转换，以及六种时间系统的相互转换。

## 使用示例

@[code](time-system-convert.cpp)

## 注意事项

- TimePoint 内部以 TAI 为基准存储，各系统转换通过偏移量实现
- UTC ↔ TAI 转换依赖闰秒表，库内置了截至 2016 年底的闰秒数据
- TDB ↔ TT 的周期项计算基于解析公式，幅度约 1.6ms
- JulianDate 用 (int day, double second) 保证精度，避免单 double 的舍入误差
- 长时间跨度建议用 TimePoint 而非 DateTime，精度更高
