
# Danmaku 2066

This is a simple shoot-em-up game built using SFML and C++. 
<img width="789" height="599" alt="screen_gp_m2066" src="https://github.com/user-attachments/assets/bb2e41d1-bea1-4e3e-8b9a-13677cbc46c7" />

## Obtaining game

Download it from [releases](https://github.com/lfc34/danmaku2066/releases/tag/danmaku2066) or build from source.

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
This will generate project files for your OS. On GNU/Linux simply run 
```bash
  make
  ./MuzhikDanmaku
```

## Game controls
Use arrows to move around, Z to shoot, and Lshift to slowdown.
