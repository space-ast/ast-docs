# 轨道根数定义及转换

## 模块概述

OrbitElement模块提供各种轨道根数（直角坐标、经典轨道根数、修正轨道根数、春分点根数、改进春分点轨道根数、球坐标根数）之间的转换函数，是天体力学和航天动力学计算的基础组件。

## 核心概念

### 轨道根数类型

模块定义了以下主要轨道根数类型：

1. **直角坐标系 (CartState)**
   - 包含位置矢量和速度矢量
   - 直观表示空间位置和运动状态

2. **经典轨道根数 (OrbElem)**
   - 长半轴 (a)
   - 偏心率 (e)
   - 轨道倾角 (i)
   - 升交点赤经 (raan)
   - 近拱点角 (argper)
   - 真近点角 (trueA)

3. **修正轨道根数 (ModOrbElem)**
   - 近拱点半径 (rp)
   - 偏心率 (e)
   - 轨道倾角 (i)
   - 升交点赤经 (raan)
   - 近拱点角 (argper)
   - 真近点角 (trueA)

4. **春分点根数 (EquinElem)**
   - 长半轴 (a)
   - h = e*sin(argper + raan)
   - k = e*cos(argper + raan)
   - p = tan(i/2)*sin(raan)
   - q = tan(i/2)*cos(raan)
   - lambda = 平经度 = M + raan + argper

5. **改进春分点轨道根数 (ModEquinElem)**
   - 半通径 (p = a(1-e^2))
   - f = e*cos(argper+raan)
   - g = e*sin(argper+raan)
   - h = tan(i/2)cos(raan)
   - k = tan(i/2)sin(raan)
   - L = raan + argper + trueA

6. **球坐标根数 (SphericalElem)**
   - 赤经 (ra)：自参考坐标系x轴逆时针量，范围 [0, 2π)
   - 赤纬 (dec)：自赤道面量，范围 [-π/2, π/2]
   - 地心距 (r) = |pos|
   - 航迹角 (fpa)：速度矢量与当地水平面的夹角，向上为正
   - 航迹方位角 (azi)：自当地北向东为正
   - 速度大小 (v) = |vel|

### 关键常数和单位

- 位置单位：米 (m)
- 速度单位：米/秒 (m/s)
- 角度单位：弧度 (rad)
- 引力参数单位：立方米/秒平方 (m³/s²)

## 用法示例

### 1. 经典轨道根数与直角坐标转换

@[code](/examples/core/orbitelement/convert_classical_scalar.cpp)

### 2. 使用类引用版本的函数

@[code](/examples/core/orbitelement/convert_classical_class.cpp)

### 3. 不同轨道根数类型之间的转换

@[code](/examples/core/orbitelement/convert_basic.cpp)

## 依赖关系

- `AstGlobal.h`: 项目全局定义
- `AstCore/Vector.hpp`: 向量类支持

## 注意事项

- 所有角度参数均使用弧度制
- 距离参数默认使用米(m)
- 时间参数默认使用秒(s)
- 引力参数单位为m³/s²
- 部分转换函数返回错误码，请适当处理错误情况
- 零偏心率轨道（圆轨道）在某些转换中可能需要特殊处理
- 改进春分点轨道根数在轨道倾角接近180度时可能存在奇异情况
- 球坐标根数在极点（|赤纬|=90°）处赤经与航迹方位角奇异，正向转换取赤经为0并保持精确往返；速度为零时航迹角与航迹方位角无定义，正向转换返回错误

## API参考

@[code](/src/AstCore/Orbit/OrbitElement.hpp)