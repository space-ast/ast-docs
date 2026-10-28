# 读取行星星历

## 概述

SpaceAST库提供了两种方式读取行星星历数据：
- JPL DE系列星历（如DE440、DE441等）
- SPICE SPK星历文件（.bsp格式）

## 读取JPL DE星历数据

使用`JplDe`类读取JPL DE系列星历数据文件。

### 功能特点

- 支持DE系列星历文件
- 支持位置和速度的读取
- 支持章动角和月球天平动相关角度的读取
- 线程安全的数据缓存机制，支持多线程并行读取

### 使用示例

@[code](planetary_ephemeris_de.cpp)

### 支持的天体

| 天体 | 枚举值 | 
|------|--------|
| 水星系质心 | `JplDe::eMercury` | 
| 金星系质心 | `JplDe::eVenus` |
| 地球 | `JplDe::eEarth` | 
| 火星系质心 | `JplDe::eMars` | 
| 木星系质心 | `JplDe::eJupiter` | 
| 土星系质心 | `JplDe::eSaturn` | 
| 天王星系质心 | `JplDe::eUranus` | 
| 海王星系质心 | `JplDe::eNeptune` | 
| 冥王星系质心 | `JplDe::ePluto` |
| 月球 | `JplDe::eMoon` | 
| 太阳 | `JplDe::eSun` | 
| 太阳系质心 | `JplDe::eSSBarycenter` | 
| 地月系质心 | `JplDe::eEMBarycenter` | 

## 读取SPK星历数据

使用`SPKParser`类读取SPICE SPK星历内核文件。

### 功能特点

- 支持SPICE SPK格式（.bsp文件）
- 直接从二进制文件读取数据，无需依赖CSPICE库


### 使用示例

@[code](planetary_ephemeris_spk.cpp)

### NAIF天体ID

SPK文件使用NAIF ID标识天体: 
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

### 注意事项

- SPK文件可以从[NAIF](https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/)获取
- 目前仅支持部分SPK数据类型
- 返回的SPK星历数据为原始坐标，未进行任何坐标系转换。请查阅所用星历文件的文档，确认数据所在的参考系
- 如果需要在多线程环境中使用，每个线程都需要创建一个SPKParser实例
