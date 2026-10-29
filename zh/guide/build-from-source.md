---
order: 7
---

<!-- 本文档已由维护者人工校对 -->

# 从源码编译

绝大多数情况下，直接从[发布页面](https://gitcode.com/space-ast/ast/releases)下载对应的二进制包即可使用。如果您需要直接从源码集成本项目，或者需要自己修改源码并编译，请按以下步骤操作：

## 获取源码

您可以通过 git 克隆本工程的最新代码：

```bash
git clone https://gitcode.com/space-ast/ast.git  --depth=1
```

如果没有安装 git，可以直接从[发布页面](https://gitcode.com/space-ast/ast/releases)下载对应版本的源码包并解压。


## 准备构建环境


- 构建工具：[xmake](https://xmake.io/zh/guide/quick-start.html)，用于实现跨平台自动构建，请参考官方文档下载并安装

- C++编译器：支持 C++11 的编译器。Windows：[MSVC](https://visualstudio.microsoft.com/)（VS2015 及以上）；Linux：GCC 5+ 或 Clang

推荐使用 [xmake](https://xmake.io/) 作为构建工具，这也是本工程默认使用的构建工具。如果不想额外安装 xmake，请参考 [其他构建方式](#其他构建方式)


## 配置

xmake 安装完成后，在终端执行 `xmake --version` 将会正常输出版本号

进入源码目录，执行以下命令配置 SpaceAST 源码工程：

```bash
xmake f -m release --packages=none 
```

- `-m release` 表示构建为发布版本，如果需要构建为调试版本，请改为 `-m debug`

- `--packages=none` 表示不下载任何外部依赖，避免在首次构建时下载大量依赖包，全部走项目内置的回退实现，依赖第三方库的目标（GUI、绘图、Python 绑定等）会被自动禁用。如果后续需要启用外部依赖，请改为 `--packages=all`


### 工程配置项

| 工程配置项 | 默认值 | 说明 |
| --- | --- | --- |
| `--with_test` | `y` | 编译 `test/` 下的单元测试 |
| `--with_examples` | `y` | 编译 `examples/` 与文档中的示例 |
| `--with_projects` | `y` | 编译 `projects/` 下的应用程序 |
| `--with_plugins` | `y` | 编译 `plugins/` 下的插件 |
| `--with_thirdparty` | `n` | 编译 `thirdparty/` 下的第三方库 |
| `--check_warnings` | `y` | 把编译警告当作错误 |
| `--system_qt` | `n` | 使用系统已安装的 Qt，而不是由 xmake 下载 |
| `--packages` | `all` | 第三方依赖策略，`none` 表示不下载任何外部依赖 |

几个常见的配置组合：

- 完整构建：默认包含所有组件和第三方依赖，但首次执行需要耗时下载依赖包
```bash
xmake f -m release
```

- 离线构建：不依赖任何第三方依赖
```bash
xmake f -m release --packages=none
```

- 警告不作为错误：方便在旧编译器上试编译
```bash
xmake f -m release --check_warnings=n
```

- 只编译核心库：跳过测试、示例、应用程序、第三方依赖，速度最快
```bash
xmake f -m release --with_test=n --with_examples=n --with_projects=n --with_plugins=n --packages=none
```

## 编译

配置完成后，执行以下命令编译 SpaceAST 源码工程：

```bash
xmake build
```

只想编译某个模块可以用 `xmake build <target>`，例如 `xmake build AstCore`

编译产物默认按平台、架构和模式分目录存放：

```
build/
└── windows/x64/release/     # Windows 下的 release 产物
└── linux/x64/release/       # Linux 下的 release 产物
```

目录里直接扁平存放了所有 `.dll`/`.so`、导入库、可执行程序和中间文件

## 运行示例

执行 `xmake run <示例名>` 运行指定的示例程序或单元测试，例如：

```bash
xmake run testState         # 运行单个示例程序（按源文件名命名）
```

::: tip
对于依赖数据的示例和测试，首次执行时会自动下载数据文件
:::



## 安装到指定目录

如果要把编译结果提供给另一个项目使用，可以把产物安装到一个独立目录：

```bash
xmake install -o ./artifacts/SpaceAST
```

目录结构与您从发布页下载的二进制包一致：

```
artifacts/SpaceAST/
├── bin/          # 可执行程序
├── include/      # 头文件（ast/、AstCore/、AstMath/ ...）
├── lib/          # 导入库
```

安装完成后，就可以按照 [快速上手](quickstart.md) 中的方式在自己的 CMake、xmake 或 Visual Studio 工程里引用它。


## 其他构建方式

在Linux上，如果不想额外安装 xmake，仓库里还附了一套 [xmake.sh](https://github.com/xmake-io/xmake.sh) 配置，适合在没有 xmake 的环境里构建，绝大部分Linux发行版都可以直接使用，无需额外安装其他工具。


```bash
./configure                         # 配置，默认 release，生成 Makefile
./configure --mode=debug            # debug 模式
make -j                             # 编译全部模块；make AstCore 只编某个模块
make install PREFIX=$PWD/artifacts  # 安装到 $PWD/artifacts 目录
```

目前这套配置只覆盖 `src/` 下的核心模块，依赖 Qt、SWIG、COM、WASM、Fortran 的目标，以及 `test/`、`examples/`、`projects/` 都还没有接入，需要这些功能时请使用 xmake 构建。


