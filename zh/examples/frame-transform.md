# 坐标变换

## 概述

地球参考系之间的坐标变换是轨道计算的基础。从惯性系（J2000/ICRF）到地固系（ECF）的变换链路由四个环节组成：

```
J2000 → MOD       岁差 (precession)
MOD   → TOD       章动 (nutation)
TOD   → GTOD      地球自转 (Earth rotation, GAST)
GTOD  → ECF       极移 (polar motion)
```

本例演示完整的变换链路、快捷函数、矩阵获取以及往返精度。

## 使用示例

@[code](frame-transform.cpp)

## 注意事项

- ECI 为地球惯性系的统称，根据配置可能指 J2000 或 ICRF
- 带速度的转换会考虑旋转坐标系的牵连速度
- 极移校正需要 EOP 数据支持
- 可通过 `aNutationMethodSet` 在 IAU1980 和 JPL DE 章动模型之间切换
