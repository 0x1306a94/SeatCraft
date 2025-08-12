
## Build Prerequisites

TGFX uses **C++17** features. Here are the minimum tools needed to build TGFX on different platforms:

- Xcode 11.0+
- GCC 9.0+
- Visual Studio 2019+
- NodeJS 14.14.0+
- Ninja 1.9.0+
- CMake 3.13.0+
- QT 5.13.0+
- NDK 19.2+ (**19.2.5345600 recommended**)
- Emscripten 3.1.58+ 


Please note the following additional notices:

- Ensure you have installed the **[Desktop development with C++]** and **[Universal Windows Platform development]** components for VS2019.
- It is **highly recommended** to use the **latest version of CMake**. Many older versions of CMake may have various bugs across different platforms.

## Dependencies

TGFX uses the [**depsync**](https://github.com/domchen/depsync) tool to manage third-party dependencies.

**For macOS platform：**

Run this script from the root of the project:

```
./sync_deps.sh
```

This script will automatically install the necessary tools and sync all third-party repositories.

**For other platforms:**

First, ensure you have the latest version of Node.js installed (you may need to restart your computer afterward). 
Then, run the following command to install the depsync tool:

```
npm install -g depsync
```

Then, run `depsync` in the project's root directory.

```
depsync
```

You might need to enter your Git account and password during synchronization. Make sure you’ve 
enabled the `git-credential-store` so that `CMakeLists.txt` can automatically trigger synchronization 
next time.