# Nebula Example

## Requirements
1. OS: Windows or Linux
2. Compiler with support for C++17.
3. GPU supporting Vulkan
4. [CMake 3.15+](https://cmake.org/download/)
5. [Vulkan SDK 1.1.130+](https://vulkan.lunarg.com/sdk/home)
6. [Python 3.5+](https://www.python.org/downloads)
    * Python modules required:
        1. numpy
        2. jedi
        
        As root/admin: `python -m pip install numpy jedi`
    * Python requirements (Windows):
        1. Correct architecture (64-bit if you're building for 64-bit systems)
        2. Installed for all users
        3. Added to PATH
        4. Installed with debugging symbols and binaries

## FIPS

Nebula uses [fips](https://floooh.github.io/fips/docs/getstarted/), which is a wrapper around CMake, as build system.

Visit https://floooh.github.io/fips/docs/getstarted/ for more information.

## Setup

#### First time setup

1. `./fips set config vulkan-win64-vstudio-debug` in your project directory
2. `./fips fetch` to fetch all dependenciescd ..

#### How to build project

Nebula uses environment variables to find resources such as assets, shaders etc.
Before building run `fips nebula` verb to set work and toolkit directory registry variables:

In your project directory:

  1. `fips nebula set work {PATH TO PROJECT}`
  2. `fips nebula set toolkit {PATH TO NEBULA}`
  3. `fips physx build`
  4. `fips anyfx setup`
  5. `fips build`
  6. `fips physx deploy`

*Note that step 5 can usually be done from within your IDE (`fips open`)*

After you've setup Physx, AnyFX and your environment variables once, you only need to (5) re-build the project in the future.

## How to compile and batch assets

Nebula has its own file formats for meshes, animations, skeletons etc. and most source files needs to be processed before being used.

Toolkit for batching assets is provided externally.

  1. Follow the setup instructions above. Make sure to set the `toolkit` environment variable to the *Nebula repository* and `work` to your project folder that has the *work* folder containing all the asset source files.
  2. Run `assetbatcher` via the terminal.

The asset batcher takes various additional arguments:

  * `-force` will force batch all assets
  * `-dir` will only batch from within a specific directory (relative to work dir)
  * `-file` will only export a specific file
  * `-system` will only export the system folder
  * `-platform` can be set to `win32` or `linux`. Assumes `win32` as default

## How to run

Either execute `fips run [target]` or run it via your IDE, ex. Visual Studio (remember to set startup project!).
