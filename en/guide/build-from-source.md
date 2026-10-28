---
order: 2
---


# Build from source

In most cases you can download the binary package from the [releases page](https://gitcode.com/space-ast/ast/releases) and use it as is. If you need to integrate the project from source, or modify and rebuild the source yourself, follow the steps below.

## Get the source

Clone the latest code with git:

```bash
git clone https://gitcode.com/space-ast/ast.git  --depth=1
```

If git is not installed, download the source archive for your version from the [releases page](https://gitcode.com/space-ast/ast/releases) and extract it.


## Prepare the build environment


- Build tool: [xmake](https://xmake.io/guide/quick-start.html), used for cross-platform automatic builds. See the official documentation to download and install it.

- C++ compiler: any compiler that supports C++11. Windows: [MSVC](https://visualstudio.microsoft.com/) (VS2015 or later); Linux: GCC 5+ or Clang.

[xmake](https://xmake.io/) is the recommended build tool and the project default. If you would rather not install xmake, see [Other build methods](#other-build-methods).


## Configure

Once xmake is installed, `xmake --version` should print a version number normally.

Enter the source directory and configure the SpaceAST project:

```bash
xmake f -m release --packages=none 
```

- `-m release` builds a release version. Use `-m debug` for a debug build.

- `--packages=none` downloads no external dependencies, avoiding a large download on the first build and falling back to the project's built-in implementations. Targets that depend on third-party libraries (GUI, plotting, Python bindings, etc.) are disabled automatically. To enable external dependencies later, switch to `--packages=all`.


### Project options

| Option | Default | Description |
| --- | --- | --- |
| `--with_test` | `y` | Build the unit tests under `test/` |
| `--with_examples` | `y` | Build `examples/` and the examples used in the documentation |
| `--with_projects` | `y` | Build the applications under `projects/` |
| `--with_plugins` | `y` | Build the plugins under `plugins/` |
| `--with_thirdparty` | `n` | Build the third-party libraries under `thirdparty/` |
| `--check_warnings` | `y` | Treat compiler warnings as errors |
| `--system_qt` | `n` | Use the system Qt instead of the Qt downloaded by xmake |
| `--packages` | `all` | Third-party dependency policy; `none` downloads no external dependencies |

A few common configurations:

- Full build: includes every component and third-party dependency, but the first run spends time downloading packages.
```bash
xmake f -m release
```

- Offline build: no third-party dependencies.
```bash
xmake f -m release --packages=none
```

- Warnings not treated as errors: handy for trying an older compiler.
```bash
xmake f -m release --check_warnings=n
```

- Core libraries only: skips tests, examples, applications and third-party dependencies, and is the fastest option.
```bash
xmake f -m release --with_test=n --with_examples=n --with_projects=n --with_plugins=n --packages=none
```

## Build

After configuration, build the SpaceAST source:

```bash
xmake build
```

To build a single module, use `xmake build <target>`, for example `xmake build AstCore`.

Build outputs are stored in separate directories by platform, architecture and mode:

```
build/
└── windows/x64/release/     # release output on Windows
└── linux/x64/release/       # release output on Linux
```

Each directory contains all `.dll`/`.so` files, import libraries, executables and intermediate files in a flat layout.

## Run examples

Use `xmake run <example>` to run an example program or unit test, for example:

```bash
xmake run testState         # run a single example program (named after the source file)
```

::: tip
Examples and tests that depend on data download it automatically the first time they run.
:::



## Install to a directory

To use the build output from another project, install it to a standalone directory:

```bash
xmake install -o ./artifacts/SpaceAST
```

The directory layout matches the binary package downloaded from the releases page:

```
artifacts/SpaceAST/
├── bin/          # executables
├── include/      # headers (ast/, AstCore/, AstMath/ ...)
├── lib/          # import libraries
```

You can then reference it from your own CMake, xmake or Visual Studio project as described in the [quick start](quickstart.md).


## Other build methods

On Linux, if you would rather not install xmake, the repository also ships an [xmake.sh](https://github.com/xmake-io/xmake.sh) configuration for environments without xmake. Most Linux distributions can use it directly without installing anything else.


```bash
./configure                         # configure (release by default), generates a Makefile
./configure --mode=debug            # debug mode
make -j                             # build all modules; make AstCore builds a single module
make install PREFIX=$PWD/artifacts  # install to $PWD/artifacts
```

This configuration currently covers only the core modules under `src/`. Targets that depend on Qt, SWIG, COM, WASM or Fortran, as well as `test/`, `examples/` and `projects/`, are not wired up yet; use xmake when you need those.
