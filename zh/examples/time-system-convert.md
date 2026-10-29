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

<!--@[code](time-system-convert.cpp) -->

