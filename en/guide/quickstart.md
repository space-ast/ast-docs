---
order: 1
---
# Quick start

```component VPBanner
title: Step-by-step tutorial
content: This tutorial is aimed at newcomers to C++. If you already have C++ experience, you can skip it.
background: var(--bg-10)
color: var(--banner-text)
actions:
  - text: Start learning
    link: /en/guide/quickstart.html#requirements
```

This tutorial walks you through downloading the SpaceAST binary release and building your first program.

## Requirements

Before you start, make sure a C++ compiler is installed.

- **Windows**: [Visual Studio](https://visualstudio.microsoft.com/) (MSVC) is the usual choice. Select the "Desktop development with C++" workload during installation.
- **Linux**: GCC is usually preinstalled. If it is not, run `sudo apt install build-essential` (Ubuntu/Debian).

The tutorial also uses a build tool later. If you are unsure which one to pick, follow the default recommendation in each section.

## Download

Go to the [releases page](https://gitcode.com/space-ast/ast/releases) and download the binary package for your operating system.

## On Windows

### 1. Extract

Extract the downloaded zip to any directory (for example `D:\SpaceAST`). The examples below assume `D:\SpaceAST`, with this layout:

```
D:\SpaceAST
├── bin/          # dynamic libraries (.dll)
├── include/      # headers
├── lib/          # import libraries (.lib)
└── examples/     # example code
```

### 2. Create a project and write code

Create a new directory as the project root (for example `D:\my_project`), next to `SpaceAST`, and add a file named `hello_ast.cpp`:

@[code](quickstart.cpp)

### 3. Build

Pick any of the following build methods:

::: tabs
@tab Visual Studio (recommended)
1. Open Visual Studio, choose **File → New → Project**, select **C++ Console App**, set the location to `D:\my_project`, and click Create.
2. In **Solution Explorer**, right-click the project → **Add → Existing Item** and select the `hello_ast.cpp` you just created.
3. Right-click the project → **Properties** and configure:
   - **C/C++ → General → Additional Include Directories**: add `D:\SpaceAST\include`
   - **Linker → General → Additional Library Directories**: add `D:\SpaceAST\lib`
   - **Linker → Input → Additional Dependencies**: add `AstCore.lib;AstUtil.lib;AstMath.lib`
   ::: tip
   This is the Release configuration. To build in Debug, switch **Configuration** at the top of the property pages to Debug, then use `AstCore_d.lib;AstUtil_d.lib;AstMath_d.lib` (library names carry the `_d` suffix).
   :::
4. Copy every `.dll` under `D:\SpaceAST\bin\` to the output directory.
    The output directory depends on your project settings, usually `D:\my_project\x64\Release\` or `D:\my_project\x64\Debug\`.
    If you are unsure, check **Linker → General → Output Directory** in the project properties.
    ::: tip
    Alternatively, add `D:\SpaceAST\bin\` to the system `PATH`.
    :::
5. Press `Ctrl+F5`, or choose **Debug → Start Without Debugging**, to build and run.
@tab CMake
1. Install CMake from [cmake.org](https://cmake.org/download/) (select "Add to PATH" during installation).
2. Create a `CMakeLists.txt` in `D:\my_project`:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloAst)

set(AST_HOME "D:/SpaceAST" CACHE PATH "SpaceAST install path")

include_directories(${AST_HOME}/include)
link_directories(${AST_HOME}/lib)

add_executable(hello_ast hello_ast.cpp)

# Debug links the _d-suffixed libraries; Release links the plain names
target_link_libraries(hello_ast
    debug     AstCore_d AstUtil_d AstMath_d
    optimized AstCore AstUtil AstMath
)
```

3. Open a terminal (PowerShell or cmd) in the project directory and run:
    ::: tip Opening a terminal
    Type `cmd` in the address bar of the `D:\my_project` folder and press Enter.
    :::

```powershell
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

4. Copy `D:\SpaceAST\bin\*.dll` to the output directory (usually `D:\my_project\build\Release\`) and run:

```powershell
.\Release\hello_ast.exe
```

@tab xmake
1. Install xmake: see the [xmake documentation](https://xmake.io/guide/quick-start.html#windows).
2. Create an `xmake.lua` in `D:\my_project`:

```lua
-- xmake.lua
add_rules("mode.release", "mode.debug")

target("hello_ast")
    set_kind("binary")
    add_files("hello_ast.cpp")

    -- Add library paths (replace if SpaceAST is not at ../SpaceAST)
    add_includedirs("../SpaceAST/include")
    add_linkdirs("../SpaceAST/lib")

    -- Link libraries (debug names carry the _d suffix)
    if is_mode("debug") then
        add_links("AstCore_d", "AstUtil_d", "AstMath_d")
    else
        add_links("AstCore", "AstUtil", "AstMath")
    end
```

3. Open a terminal (PowerShell or cmd) in the project directory and run:
    ::: tip Opening a terminal
    Type `cmd` in the address bar of the `D:\my_project` folder and press Enter.
    :::

```powershell
xmake f -m release
xmake
xmake run hello_ast
```
:::

::: tip Notes
- Debug library names carry the `_d` suffix (for example `AstCore_d.dll`); Release names do not.
- Link the modules your program uses. `AstCore` (dynamics), `AstMath` (math) and `AstUtil` (utilities) are the three base layers.
- The DLLs must be next to the executable or on `PATH` at run time.
:::

## On Linux

### 1. Extract

Extract the downloaded `tar.gz` to any directory (for example `/opt/SpaceAST`). The examples below assume `/opt/SpaceAST`, with this layout:

```
/opt/SpaceAST/
├── include/      # headers
├── lib/          # shared libraries (.so)
└── examples/     # example code
```

### 2. Create a project and write code

Create a project directory (for example `~/my_project`) and add a file named `hello_ast.cpp`:

@[code](quickstart.cpp)

### 3. Build

Pick any of the following build methods:

::: tabs
@tab CMake
1. Install CMake if it is not already available:
```bash
sudo apt install cmake
```

2. Create a `CMakeLists.txt` in `~/my_project`:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(HelloAst)

set(AST_HOME "/opt/SpaceAST" CACHE PATH "SpaceAST install path")

include_directories(${AST_HOME}/include)
link_directories(${AST_HOME}/lib)

add_executable(hello_ast hello_ast.cpp)
target_link_libraries(hello_ast AstCore AstUtil AstMath)

# Set the runtime library search path
set_target_properties(hello_ast PROPERTIES
    INSTALL_RPATH "${AST_HOME}/lib"
    BUILD_RPATH "${AST_HOME}/lib"
)
```

3. Open a terminal in the project directory and run:
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./hello_ast
```

@tab Makefile
1. Make sure `make` and `g++` are installed (usually preinstalled; otherwise run `sudo apt install build-essential`).
2. Create a `Makefile` in `~/my_project`:

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

3. Open a terminal in the project directory and run:
```bash
make
./hello_ast
```

@tab xmake
1. Install xmake: see the [xmake documentation](https://xmake.io/guide/quick-start.html#linux).
2. Create an `xmake.lua` in `~/my_project`:

```lua
-- xmake.lua
add_rules("mode.release", "mode.debug")

target("hello_ast")
    set_kind("binary")
    add_files("hello_ast.cpp")

    -- Add library paths (replace if SpaceAST is not at /opt/SpaceAST)
    add_includedirs("/opt/SpaceAST/include")
    add_linkdirs("/opt/SpaceAST/lib")
    add_rpathdirs("/opt/SpaceAST/lib")

    add_links("AstCore", "AstUtil", "AstMath")
```

3. Open a terminal in the project directory and run:
```bash
xmake f -m release
xmake
xmake run hello_ast
```
:::

::: tip Notes
- `rpath` lets the program find the `.so` files at run time without setting `LD_LIBRARY_PATH` manually.
:::

## Next steps

Congratulations! You have run your first SpaceAST program.

- Browse the [examples](../examples/) for more example code
- Read the [module reference](../reference/) to learn what each module does
- See [build from source](build-from-source.md) to integrate and build the source yourself
- Check the [API reference](https://space-ast.github.io/ast/api/) for the full interface
