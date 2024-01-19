# Project
The game 

# Build
## Requirements:
| Tool            | Description                                                               |
| --------------- | ------------------------------------------------------------------------- |
| `Powershell`    | need to enable scripts with `Set-ExecutionPolicy RemoteSigned`.           |
| `CMake`         | need to be in the path, version 3.12 at least.                            |
| `Visual Studio` | with MSbuild 2022 (can be changed but cross your fingers, it's untested). |

## How to build:
Use the command `./generate.ps1` and hope for the best.

Available arguments are:
| Argument     | Description                            |
| ------------ | -------------------------------------- |
| `-Cleanup`   | Clean generated files                  |
| `-NoBuild`   | Don't build executables and libraries  |
| `-NoInstall` | Don't install resources                |
| `-NoCleanup` | Don't clean resources                  |
| `-VS2019`    | Use Visual Studio 2019 instead of 2022 |


## Generation:
- Visual Studio solution in `./_solution/Game.sln`.
- Build results are in `./_output/[bin|lib]/[debug|release]/{binaries}`.

# Dependencies:
| Library                                                | Description                                             |
| ------------------------------------------------------ | ------------------------------------------------------- |
| `Glfw` (Windows and inputs helper)                     | Official website: https://www.glfw.org                  |
| `Glad` (Get entry points for `opengl32.dll`'s methods) | Generated at: https://glad.dav1d.de                     |
| `GLM`  (OpenGL Mathematics)                            | Clone: https://github.com/g-truc/glm                    |
| `stb_image` (Load image)                               | Clone: https://github.com/nothings/stb                  |
| `FreeType` (Font renderer)                             | Clone: https://gitlab.freedesktop.org/freetype          |
| `Assimp` (Model loader)                                | Official website: https://assimp.org/                   |
| `ReactPhysics3D` (Physic Engine)                       | Clone: https://github.com/DanielChappuis/reactphysics3d |
| `ImGui` (Gui Engine)                                   | Clone: https://github.com/ocornut/imgui                 |
