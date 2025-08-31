# Raycaster Engine - SDL3

A Wolfenstein-inspired raycaster engine using **SDL3** for graphics and rendering built in C.

---

## Features

- Simple Raycaster Rendering
- Debug Menu
  
---

## Requirements

- **C Compiler** (g++ recommended)  
- **SDL3 Development Libraries**  
- **MinGW** (for Windows builds)  
- Optional: **CMake** if building via CMake
  
---

## Project Structure
```bash
───src
    ├───include
    │   └───SDL3
    └───lib
        ├───cmake
        │   └───SDL3
        └───pkgconfig
```

## Compilation Instructions

include makefile in the same directory as c file along with SDL3 dll and libraries structured as above and compile using this

```
all:
	g++ -Wextra -Wpedantic -I src/include -L src/lib -o main main.c -lmingw32 -lSDL3

```
