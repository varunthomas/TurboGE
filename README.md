# TurboGE [![License](https://img.shields.io/github/license/varunthomas/TurboGE)](https://github.com/varunthomas/TurboGE/blob/main/LICENSE)

TurboGE is a small game engine created in C++20 using OpenGL rendering. It uses Python as scripting language. This is made purely for learning purposes.

***

## Getting Started
TurboGE is created in Windows environment and other OS are not considered at the moment.
Recommended version of Visual Studio: 2022

<ins>**1. Download source code:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/varunthomas/TurboGE`.

<ins>**2. Generate the solution:**</ins>

1. Run the [GenerateProjects.bat](https://github.com/varunthomas/TurboGE/blob/main/GenerateProjects.bat) file in the root directory. This will run the premake required to build the project. This will also create the Visual Studio solutions file.


***

### Main features:
- Python Scripting (Pyscript)
- Quad rendering
- Sprite rendering
- Batching
- Blending
- Editor App to change the transforms, sprite and camera at runtime.
- Orthographic and Perspective camera
- Free moving camera for editor.

**References:**

- Cherno Game Engine
- learnopengl.com
- Sample python script can be referenced in Editor/scripts/sample.py

**Screenshots:**
![alt text](https://github.com/varunthomas/TurboGE/blob/TurboGE_dev/Screenshot1.png?raw=true)
