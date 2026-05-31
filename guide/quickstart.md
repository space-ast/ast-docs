---
order: 1
---
# 快速入门

```component VPBanner
title: 逐步教程
content: 本教程针对不熟悉 C++ 的新手。如果你已经有相关经验，可跳过该教程。
background: var(--bg-10)
color: var(--banner-text)
```

本教程将引导你下载 SpaceAST 的二进制发布包，并在自己的项目中使用它。

## 环境要求

在开始之前，请确保已经安装了任意一个 C++ 编译器。

- **Windows**：一般使用 [Visual Studio](https://visualstudio.microsoft.com/)（即 MSVC），安装时勾选"使用 C++ 的桌面开发"工作负载
- **Linux**：系统一般自带有 GCC 编译器，如果没有，运行 `sudo apt install build-essential`（Ubuntu/Debian）安装

本教程后续会用到构建工具。如果你不确定选哪个，跟着教程的默认推荐走即可。

## 下载

前往 [GitHub Releases](https://github.com/space-ast/ast/releases) 页面，根据你的操作系统下载对应的二进制包。

## Windows 下使用

### 1. 解压

将下载的 zip 解压到任意目录（如 `D:\SpaceAST`），以下示例都假设解压到 `D:\SpaceAST`，目录结构如下：

```
D:\SpaceAST
├── bin/          # 动态库（.dll）
├── include/      # 头文件
├── lib/          # 导入库（.lib）
└── examples/     # 示例代码
```

### 2. 创建项目并编写代码

创建一个新目录作为项目根目录（如 `D:\my_project`），与 `SpaceAST` 平级，并在其中新建一个 `hello_ast.cpp` 文件，内容如下：

@[code](quickstart.cpp)

### 3. 构建

选择任意一种构建方式：

::: tabs
@tab Visual Studio（推荐）
1. 打开 Visual Studio，点击 **文件 → 新建 → 项目**，选择 **C++ 控制台应用**，将位置设为 `D:\my_project`，点击创建
2. 在 **解决方案资源管理器**中，右键单击项目 → **添加 → 现有项**，选择刚才创建的 `hello_ast.cpp`
3. 右键项目 → **属性**，进行以下配置：
   - **C/C++ → 常规 → 附加包含目录**，添加 `D:\SpaceAST\include`
   - **链接器 → 常规 → 附加库目录**，添加 `D:\SpaceAST\lib`
   - **链接器 → 输入 → 附加依赖项**，添加 `AstCore.lib;AstUtil.lib;AstMath.lib`
   ::: tip
   以上是 Release 模式的配置。如果要用 Debug 模式，先将属性页顶部的 **配置** 下拉框切换为 Debug，然后在附加依赖项中填入 `AstCoreD.lib;AstUtilD.lib;AstMathD.lib`（库名带 `D` 后缀）。
   :::
4. 将 `D:\SpaceAST\bin\` 下的所有 `.dll` 文件拷贝到输出目录。
    输出目录取决于你的项目配置，通常为 `D:\my_project\x64\Release\` 或 `D:\my_project\x64\Debug\`。
    如果不确定具体路径，可以在项目属性 **链接器 → 常规 → 输出目录** 中查看。
    ::: tip
    你也可以直接将路径 `D:\SpaceAST\bin\`添加到系统 `PATH` 环境变量。
    :::
5. 按 `Ctrl+F5` 或点击菜单栏的 **调试 → 开始执行**，编译并运行
@tab CMake
1. 安装 CMake：从 [cmake.org](https://cmake.org/download/) 下载安装（安装时勾选"添加到 PATH"）
2. 在 `D:\my_project` 目录下新建一个 `CMakeLists.txt` 文件，内容如下：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloAst)

set(AST_HOME "D:/SpaceAST" CACHE PATH "SpaceAST install path")

include_directories(${AST_HOME}/include)
link_directories(${AST_HOME}/lib)

add_executable(hello_ast hello_ast.cpp)

# Debug 模式链接 D 后缀库；Release 链接不带 D
target_link_libraries(hello_ast
    debug     AstCoreD AstUtilD AstMathD
    optimized AstCore AstUtil AstMath
)
```

3. 然后在项目目录下打开终端（PowerShell 或 cmd ），依次执行：
    ::: tip 如何打开终端
    在 `D:\my_project` 文件夹的地址栏中输入 `cmd` 并回车
    :::

```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

4. 将 `D:\SpaceAST\bin\*.dll` 拷贝到输出目录（通常是 `D:\my_project\build\Release\`），然后运行：

```powershell
.\Release\hello_ast.exe
```

@tab xmake
1. 安装 xmake：参考 [xmake 官方文档](https://xmake.io/zh/guide/quick-start.html#windows)
2. 在 `D:\my_project` 目录下新建一个 `xmake.lua` 文件，内容如下：

```lua
-- xmake.lua
add_rules("mode.release", "mode.debug")

target("hello_ast")
    set_kind("binary")
    add_files("hello_ast.cpp")

    -- 添加库路径（如果 SpaceAST 不在 ../SpaceAST，请替换为实际路径）
    add_includedirs("../SpaceAST/include")
    add_linkdirs("../SpaceAST/lib")

    -- 链接库（debug 模式库名带 D 后缀）
    if is_mode("debug") then
        add_links("AstCoreD", "AstUtilD", "AstMathD")
    else
        add_links("AstCore", "AstUtil", "AstMath")
    end
```

3. 然后在项目目录下打开终端（PowerShell 或 cmd ），依次执行：
    ::: tip 如何打开终端
    在 `D:\my_project` 文件夹的地址栏中输入 `cmd` 并回车
    :::

```powershell
xmake f -m release
xmake
xmake run hello_ast
```
:::

::: tip 提示
- Debug 模式下库名带 `D` 后缀（如 `AstCoreD.dll`），Release 模式不带
- 你的程序依赖哪些模块就链接哪些库：`AstCore`（动力学）、`AstMath`（数学）、`AstUtil`（工具）是最基础的三层
- 运行前必须确保 DLL 在可执行文件同目录或 `PATH` 中
:::

## Linux 下使用

### 1. 解压

将下载的 `tar.gz` 解压到任意目录（如 `/opt/SpaceAST`），以下示例都假设解压到 `/opt/SpaceAST`，目录结构如下：

```
/opt/SpaceAST/
├── include/      # 头文件
├── lib/          # 链接库（.so）
└── examples/     # 示例代码
```

### 2. 创建项目并编写代码

创建项目目录（如 `~/my_project`），并在其中新建一个 `hello_ast.cpp` 文件，内容如下：

@[code](quickstart.cpp)

### 3. 构建

选择任意一种构建方式：

::: tabs
@tab CMake
1. 安装 CMake（如果尚未安装）：
```bash
sudo apt install cmake
```

2. 在 `~/my_project` 目录下新建 `CMakeLists.txt` 文件，内容如下：

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloAst)

set(AST_HOME "/opt/SpaceAST" CACHE PATH "SpaceAST install path")

include_directories(${AST_HOME}/include)
link_directories(${AST_HOME}/lib)

add_executable(hello_ast hello_ast.cpp)
target_link_libraries(hello_ast AstCore AstUtil AstMath)

# 设置运行时库搜索路径
set_target_properties(hello_ast PROPERTIES
    INSTALL_RPATH "${AST_HOME}/lib"
    BUILD_RPATH "${AST_HOME}/lib"
)
```

3. 在项目目录下打开终端，依次执行以下命令：
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./hello_ast
```

@tab Makefile
1. 确保已安装 make 和 g++（一般系统自带，如果没有则运行 `sudo apt install build-essential`）
2. 在 `~/my_project` 目录下新建 `Makefile` 文件，内容如下：

```makefile
# Makefile
AST_HOME := /opt/SpaceAST
CXX      := g++
CXXFLAGS := -std=c++11 -I$(AST_HOME)/include
LDFLAGS  := -L$(AST_HOME)/lib -Wl,-rpath,$(AST_HOME)/lib
LDLIBS   := -lAstCore -lAstUtil -lAstMath

hello_ast: hello_ast.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
```

3. 在项目目录下打开终端，执行以下命令：
```bash
make
./hello_ast
```

@tab xmake
1. 安装 xmake：参考 [xmake 官方文档](https://xmake.io/zh/guide/quick-start.html#linux-%E5%8F%91%E8%A1%8C%E7%89%88)
2. 在 `~/my_project` 目录下新建 `xmake.lua` 文件，内容如下：

```lua
-- xmake.lua
add_rules("mode.release", "mode.debug")

target("hello_ast")
    set_kind("binary")
    add_files("hello_ast.cpp")

    -- 添加库路径（如果 SpaceAST 不在 /opt/SpaceAST，请替换为实际路径）
    add_includedirs("/opt/SpaceAST/include")
    add_linkdirs("/opt/SpaceAST/lib")
    add_rpathdirs("/opt/SpaceAST/lib")

    add_links("AstCore", "AstUtil", "AstMath")
```

3. 在项目目录下打开终端，依次执行以下命令：
```bash
xmake f -m release
xmake
xmake run hello_ast
```
:::

::: tip 提示
- `rpath` 让程序运行时自动搜索 `.so` 文件的路径，无需手动设置 `LD_LIBRARY_PATH`
:::

## 下一步

恭喜！你已经成功运行了第一个 SpaceAST 程序。

- 浏览 [examples/](../examples/) 了解更多示例代码
- 阅读各模块的[技术文档](../technical/)深入了解具体功能
- 查看 [API 手册](https://space-ast.github.io/ast/api/) 获取完整接口说明
