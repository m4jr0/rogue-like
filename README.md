# Rogue Like

## Overview

**Rogue Like** is a minimalist video game built in **C++20**, designed to use as few external libraries as possible.

It uses:
* [**OpenGL**](https://www.opengl.org/): primary rendering backend, used as a thin, immediate layer over the engine so it can be replaced with [**Vulkan**](https://www.vulkan.org/) or [**DirectX 12**](https://learn.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide) in future versions without rewriting core systems.
* [**GLAD**](https://github.com/Dav1dde/glad): OpenGL function loader that keeps the project self-contained and avoids platform-specific extension mechanisms.
* [**GLFW**](https://www.glfw.org/): cross-platform windowing, input, and context creation, replacing OS-specific code (Win32, X11, etc.) with a small, consistent API.
* [**miniaudio**](https://miniaud.io/): tiny, dependency-free audio backend for sound and music, offering cross-platform playback without relying on larger audio frameworks.

## Build the project

The project can be built on both Windows and Linux (it's been developed on/tested with **MSVC** and **GCC**, respectively), but because it is in active development, some manual steps might be required beforehand (like installing specific packages).

Also, you need to generate engine resources from the raw assets before building or running (see next section).

### With Visual Studio 2022

First, install the **C++ CMake tools for Windows** component via the **Visual Studio Installer**. Then, open the cloned project directly from **Visual Studio 2022**.

### With Visual Studio Code

Open the project with **Visual Studio Code**, and with **CTRL+SHIFT+P** execute the `CMake: Configure` command. Then, launch **Comet** with the **Comet (build) [Windows/Unix]** configuration.

**Note:** you will need to install the **[C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)** and **[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)** extensions.

### With CMake only

On Linux, open a terminal and enter the following commands:
* `git clone https://github.com/m4jr0/rogue-like.git`
* `cd rogue-like && mkdir build && cd build`
* `cmake ..`
* `cmake --build .`

It should be pretty similar on Windows.

## Generating resources

In this project, "resources" refer to processed assets (textures, models, audio, etc.) stored in an engine-specific format. They're generated via a Python script/tooling layer that was implemented quickly to prioritize engine/game development over building a full standalone editor/tool suite.

To generate the resources, follow these steps:

1. Install **Python 3.x** (with `venv` support).
2. `cd /path/to/rogue-like/tools/`
3. `python3 -m venv .venv`
4. `source .venv/bin/activate`
5. `pip install -r requirements.txt`
6. `python -m rlres.asset_pipeline --src ../assets --out ../build/resources`

**Notes:**
* On Windows, use `.\.venv\Scripts\activate` for step **4**.
* You can also open `/path/to/rogue-like/tools/` in Visual Studio Code and run the pipeline from there (after configuring the Python interpreter).

### Aseprite assets

If you want to generate sprite assets directly from **Aseprite**, follow the guide available [here](https://github.com/m4jr0/rogue-like/blob/main/docs/ASEPRITE.md).
## Assets

## Environment assets
* From Pixel Poem's [**Dungeon Tileset II**](https://pixel-poem.itch.io/dungeon-assetpuck).

## Character assets
* From Mattz Art's [**Adventurer 2D Top-Down**](https://xzany.itch.io/top-down-adventurer-character).
* From Zerie's [**Tiny RPG Character Asset Pack v1.03**](https://zerie.itch.io/tiny-rpg-character-asset-pack).

## Sounds (SFX/Music)
* From alkakrab's [**500+ Tracks Mega Bundle**](https://alkakrab.itch.io/270bundle).

## License

This project is licensed under the [GNU General Public License v3.0](https://github.com/m4jr0/rogue-like/blob/main/LICENSE).
