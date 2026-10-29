---
order: 5
---

<!-- 本文档已由维护者人工校对 -->

# 常见问题排查

该文档按“构建 → 运行 → 初始化 → 结果”的顺序列出常见问题及解决方案。

您也可以直接向我们反馈您遇到的问题：

- 问题反馈：[Issues](https://gitcode.com/space-ast/ast/issues)

- 使用讨论：[Discussions](https://gitcode.com/space-ast/ast/discussions)。

如果能附上以下信息，可以显著缩短排查时间：

- 一段能复现问题的代码；

- 完整的日志和报错信息。


## 构建与链接

### 找不到头文件

```
fatal error: ast/DateTime.hpp: No such file or directory
```

确认项目配置了包含目录 `<SpaceAST>/include`，而不是 `bin` 或解压后的上层目录。


### LNK2019 / undefined reference

符号未解析，通常是下面几种原因：

- **少链接了模块**：例如用了 `aTwoBodyProp()` 却没链接 `AstCore`。linux 还需要将依赖链上的库一起链接，例如 `AstCore` 需要 `AstMath`、`AstWeather`、`AstScript`、`AstUtil`。

- **Debug/Release 混用**：Windows 上 Debug 库名带 `_d`，检查 Debug 模式下的附加依赖项是否配置正确，例如 `AstCore_d.lib`。

- **ABI 宏不一致**：自己定义了 `AST_USE_EIGEN_MATRIX`、关闭了 `AST_ENABLE_NAMESPACE`，或定义了 `AST_BUILD_LIB_*`，删除这些定义后重新编译。

- **架构不匹配**：x86 工程链接 x64 库。检查项目平台和库目录。

定位具体符号：

```powershell
# Windows：查看 DLL 导出了哪些符号
dumpbin /EXPORTS AstCore.dll | findstr aTwoBodyProp
```

```bash
# Linux
nm -D /opt/SpaceAST/lib/libAstCore.so | grep aTwoBodyProp
ldd ./my_app
```

### LNK2038 RuntimeLibrary 不匹配

典型报错：

```
LNK2038: 检测到“RuntimeLibrary”的不匹配项: 值“MDd_DynamicDebug”不匹配值“MT_StaticRelease”
```

Windows 发布库使用 `/MD`（Release）和 `/MDd`（Debug）。在项目属性的 **C/C++ → 代码生成 → 运行库** 中改成对应的动态库版本，不要使用 `/MT` 或 `/MTd`。

### 运行库、头文件或架构不一致

- 发布库在 Release 下使用 `/MD`，Debug 下使用 `/MDd`，你的工程必须保持一致。

- 32 位程序不能链接 64 位库；x64 与 ARM64 也不能混用。

- 升级库时，头文件、导入库和 DLL/SO 要**一起替换**，只换其中一部分会出现符号找不到或运行时崩溃。

### 源码构建时警告被当作错误

从源码编译时默认 `--check_warnings=y`，旧编译器上可能因为警告直接失败。可以临时关闭：

```bash
xmake f --check_warnings=n
```

### xmake 下载第三方依赖失败

首次完整构建会下载大量依赖。网络受限时改用离线配置：

```bash
xmake f --packages=none
```

这会禁用 GUI、绘图、Python 绑定等依赖第三方库的目标，但核心库和测试仍可构建。

## 运行失败

### Windows 下 0xc000007b 或 0xc0000135

- `0xc0000135`（STATUS_DLL_NOT_FOUND）：找不到依赖的 DLL。把 `<SpaceAST>/bin` 加入 `PATH`，或把 DLL 拷贝到 exe 同目录。

- `0xc000007b`（STATUS_INVALID_IMAGE_FORMAT）：通常是位数不匹配，例如 64 位程序加载了 32 位 DLL，或混入了不同架构的第三方库。

用 `dumpbin /dependents my_app.exe` 查看依赖，再逐个确认 DLL 是否存在、架构是否一致。

### Linux 下找不到 .so

```
error while loading shared libraries: libAstCore.so: cannot open shared object file
```

处理方式任选一种：

```bash
# 注意将下面命令中的 `库的安装目录` 替换为实际的库安装目录，例如 /opt/SpaceAST/

# 临时运行
export LD_LIBRARY_PATH=库的安装目录/lib:$LD_LIBRARY_PATH

# 或给可执行文件加 rpath（CMake）
set_target_properties(my_app PROPERTIES BUILD_RPATH "库的安装目录/lib")

# 检查依赖是否齐全
ldd ./my_app
```

### 程序启动就崩溃，或 Debug 下断言失败

检查是否 Debug/Release 混用，Windows 上要求运行库（`/MD` 与 `/MDd`）一致，混用可能导致堆被不同运行库管理，表现为释放时崩溃或断言。

## 初始化与数据

### 离线环境下首次运行卡住

在数据目录为空时会尝试自动下载，离线部署时需要手动下载[数据文件](https://gitcode.com/space-ast/ast-data/tags)。

详细说明见[数据与运行时](data-and-runtime.md)。

## 结果不符合预期

这类问题通常没有报错，只是数值不对。先对照下表：

| 现象 | 常见原因 |
| --- | --- |
| 位置、半径差 1000 倍 | 把千米当成米，或反过来 |
| 角度差约 57.3 倍 | 把度当成弧度，或反过来 |
| 时间差 37 秒左右 | 把 UTC 当成 TAI，或忽略了闰秒 |
| 时间差不到 1 秒 | UT1 与 UTC 混淆，或 EOP 未加载 |
| 速度明显不对，位置正常 | 用了只转换位置的重载，没有转换速度 |
| 轨道方向、升交点明显偏移 | J2000/ECF/ICRF 等坐标系用混了 |
| 长时间传播结果发散 | 传播模型与任务不匹配，或时长单位用成了天 |
| 行星位置相差较大 | 星历文件、参考系或时间尺度不一致 |


更多约定见[单位与约定](units-and-conventions.md)。

如果问题是本项目的内部计算逻辑导致的，请向我们[反馈](https://gitcode.com/space-ast/ast-data/issues/new)。

## 中文输出乱码

这种情况一般出现在 Windows 上，常见就是以下两种原因：

- 程序内部字符串为 GBK 编码，但是程序的本地化配置对应的编码是 UTF-8。
- 程序内部字符串为 UTF-8 编码，但是程序的本地化配置对应的编码是 GBK。

本项目推荐您使用 UTF-8 编码，这将更为现代化。

您需要同时做到：

1. 将程序内部字符串存储为 UTF-8 编码
    - 将源文件保存为 UTF-8，如果编译器较老，则保存为 UTF-8 BOM；
    - 在编译选项加 `/utf-8`，如果因为编译器较老不支持该选项，则在源码里添加 `#pragma execution_character_set("utf-8")`；
2. 设置您程序的本地化配置，让标准库按照 UTF-8 编码处理字符串
    - 通过 `setlocale(LC_ALL, ".UTF-8")` 修改您程序的本地化配置，此后该程序中 `printf` 等标准库函数将按照 UTF-8 编码处理字符串。

::: info
如果您的操作系统低于 Windows 10 版本 1803，`setlocale(LC_ALL, ".UTF-8")` 将会失败。
此时推荐直接使用库自带的打印函数 `posix::printf()` 替换 `printf()`，它能很好地处理 Windows 下 UTF-8 编码字符串的打印输出问题。

```cpp
#include "ast/IO.hpp"

int main()
{
    posix::printf(u8"你好，世界！\n");
}
```
:::


