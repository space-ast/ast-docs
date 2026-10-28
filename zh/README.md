---
home: true
icon: house
title: 项目概览
footer: false
heroText: SpaceAST
tagline: 面向航天仿真的现代 C++ 算法库
heroImage: /hero.svg
heroAlt: SpaceAST 轨道与卫星插画
actions:
  - text: 快速上手
    link: /zh/guide/quickstart.html
    type: primary
  - text: 模块参考
    link: /zh/reference/
  - text: API 手册
    link: https://space-ast.github.io/ast/api/
  - text: 下载
    link: https://gitcode.com/space-ast/ast/releases/
features:
  - title: 高精度轨道传播
    details: 二体、J2/J4 解析传播与 HPOP 数值传播，支持多种摄动力模型。
    icon: satellite
    link: /zh/reference/core/propagator/
  - title: 完整的坐标与时间体系
    details: J2000/ECEF 变换链、儒略日与 UTC/TAI/TT 转换、JPL DE 与 SPK 星历读取。
    icon: clock
    link: /zh/reference/core/time.html
  - title: 姿态与数学基础
    details: 四元数、欧拉角、方向余弦矩阵，以及线性代数、ODE 积分与插值。
    icon: compass
    link: /zh/reference/math/attitude.html
  - title: 模块化共享库
    details: core、math、util 等独立模块按需链接，依赖关系清晰。
    icon: cubes
    link: /zh/reference/
  - title: 跨平台构建
    details: 支持 Windows 与 Linux，兼容 C++11，可使用 xmake 或 CMake 集成。
    icon: laptop-code
    link: /zh/guide/build-from-source.html
  - title: Python 绑定
    details: 通过 SWIG 暴露核心接口，便于脚本化验证与二次开发。
    icon: code
    link: https://github.com/space-ast/ast
---

## 开始使用

从 [快速上手](guide/quickstart.html) 获取发布包并完成第一个程序，或直接浏览 [模块参考](reference/) 与 [示例集](examples/)

> SpaceAST（/æst/）专注于为航天任务分析提供可复用的算法组件
