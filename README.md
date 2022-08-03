# Butano template

GBA gamedev template for [Butano engine](https://github.com/GValiente/butano).

This template is meant to be used on [VSCode](https://code.visualstudio.com/), with [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) & [Native Debug](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) extensions.


## Changing paths

You need to change some paths in this template so that they suit your environment.
1. Change `LIBBUTANO :=` on [Makefile](Makefile#L30) to your location for Butano.
2. Change `-I` lines on [compile_flags.txt](compile_flags.txt#L3) to your location for Butano.


## Setting up debugger

[launch.json](launch.json) and [tasks.json](tasks.json) are provided to easily debug your game with [mGBA](https://mgba.io/).

1. Add `mgba` and `devkitARM/bin/arm-none-eabi-gdb` executables to your `PATH`.
2. Install [Native Debug extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug) on VSCode.
3. Settings done; Hit F5 on VSCode to debug.

Optionally, add `-O0` to `USERFLAGS` on Makefile to improve debugging.\
Don't forget to remove `-O0` before your actual release!

(Unfortunately, giving `-O0` sometimes prevents your game from launching.)
