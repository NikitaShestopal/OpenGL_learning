# OpenGL 3D Graphics Engine

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B_Core-red.svg?logo=opengl&logoColor=white)](https://www.opengl.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15%2B-064F8C.svg?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

> **Modular 3D rendering engine built from scratch in modern C++ utilizing OpenGL 3.3 Core Profile, GLFW, GLAD, and GLM. Designed for low-level graphics pipeline exploration, real-time spatial transformations, and modular model rendering.**

---

## Architecture & Features

- **Modern C++ & Clean Architecture:** Built on modern C++ conventions with modular object-oriented encapsulation for shaders, camera controls, mesh processing, and model loading.
- **Programmable Graphics Pipeline:** Custom GLSL shaders (Vertex and Fragment) managing vertex attributes, MVP (Model-View-Projection) matrix pipelines, and surface rasterization.
- **6-DOF Interactive Camera:** Quaternion- and Euler-based view transform system with mouse pitch/yaw tracking and responsive keyboard input navigation.
- **High-Performance Spatial Mathematics:** Powered by **GLM** for vectorization, transformation matrices, and linear algebra routines.
- **Model Loading & Resource Management:** Support for parsing `.obj` wavefront meshes with material definitions (`.mtl`) and dynamic vertex attribute arrays (VAO, VBO, EBO).
- **Self-Contained Dependency Toolchain:** Bundled and configured with **GLFW**, **GLAD**, and **GLM** under a unified CMake build system.

---

## Rendering Pipeline

```
       [ 3D Model (.obj) ] ──► [ Mesh Parser ]
                                     │
                                     ▼
 ┌─────────────────────────── [ VAO / VBO / EBO ] ───────────────────────────┐
 │                                                                            │
 │   Vertex Shader (GLSL)                                                    │
 │   Local Space ──► World Matrix ──► View (Camera) ──► Projection (Perspective)
 │                                                                            │
 │   Rasterization & Interpolation                                            │
 │                                                                            │
 │   Fragment Shader (GLSL)                                                   │
 │   Surface Color ──► Lighting / Material Calculation ──► Framebuffer Target │
 └────────────────────────────────────────────────────────────────────────────┘
                                     │
                                     ▼
                          [ GLFW Window Context ]
```

---

## Tech Stack & Dependencies

- **Language:** C++20
- **Build System:** CMake
- **Graphics API:** OpenGL 3.3 Core Profile
- **Windowing & Input:** GLFW 3.4
- **Function Loader:** GLAD
- **Math Library:** GLM (OpenGL Mathematics)

---

## Building & Running

### Prerequisites

Ensure you have installed:
- Modern C++ compiler supporting C++17 or C++20 (`g++`, `clang++`, or MSVC 2019+)
- CMake (>= 3.15)
- Standard OpenGL runtime drivers for your GPU

### Clone the Repository

```
git clone https://github.com/your-username/OpenGL_learning-Main_Engine.git
cd OpenGL_learning-Main_Engine
```

### Build via CMake

```
# Generate build files
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compile project
cmake --build build --config Release
```

### Run Executable

- **Linux / macOS:**
  ```
  ./build/OpenGL_learning
  ```
- **Windows:**
  ```
  .\build\Release\OpenGL_learning.exe
  ```

---

## Controls

| Input | Action |
| :--- | :--- |
| <kbd>W</kbd> / <kbd>S</kbd> | Move Forward / Backward |
| <kbd>A</kbd> / <kbd>D</kbd> | Strafe Left / Right |
| <kbd>Mouse</kbd> | Free-look Pitch and Yaw orientation |
| <kbd>Scroll</kbd> | Camera Field of View (Zoom in/out) |
| <kbd>ESC</kbd> | Close window / Release capture |

---

## Repository Structure

```
OpenGL_learning-Main_Engine/
├── external/           # Vendored dependencies (GLFW, GLAD, GLM)
├── resources/          # 3D models (.obj), textures, materials (.mtl)
│   ├── Car/
│   ├── Minion/
│   ├── moto/
│   └── sofa/
├── src/                # Engine source code
│   ├── Camera.h        # View matrix and Euler orientation controller
│   ├── Mesh.h          # Vertex buffer mapping & draw calls
│   ├── Model.h         # Multi-mesh loading and hierarchy
│   ├── Shader.h        # GLSL program compilation and uniform binding
│   ├── main.cpp        # Application initialization and main render loop
│   └── *.glsl          # Vertex and fragment shader code
├── CMakeLists.txt      # Root build configuration
└── README.md
```
