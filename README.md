
# Danmaku 2066

This is a simple game built using SFML and C++.

# Obtaining game

## Download from releases

Download game from releases

## Building from source

Before building from source on linux, consider [this](https://www.sfml-dev.org/tutorials/2.6/start-cmake.php#requirements). There are packages that you need to install before building.

1. Clone the project

```bash
  git clone https://github.com/lfc34/danmaku2066.git
  cd danmaku2066/
```

2. Create build/ folder

```bash
  mkdir build
  cd build/
```

3. Run CMake like this 

```bash
  cmake .. 
```
This will generete project files for your OS. On GNU/Linux simply run 
```bash
  make
  bin/MuzhikDanmaku
```

> **Warning** building and running this game wasn't tested on MS Windows.  

For Microsoft Windows it will build Visual Studio solutions (or MinGW, depends on what you pick), so refer to this [documentation (Visual Studio)](https://learn.microsoft.com/en-us/visualstudio/ide/building-and-cleaning-projects-and-solutions-in-visual-studio?view=vs-2022). 

    