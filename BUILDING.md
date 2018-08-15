# Building Ratscript

## Building Basics

CMake 3.1 or later is used to build the Ratscript library and console. For your
convenience, we've included Makefiles to automate all common build scenarios on
Linux. Run `$ make` in the root of this repository for more information.

At this time, the Makefiles are only designed for Linux. If you are building
on another system, you can interact with CMake directly.

Currently, Ratscript is only designed to be built by GCC (5.3 or later) or
Clang (3.4 or later).

## Building and Linking Dependencies

Ratscript relies on CPGF and PawLIB. The default behavior of the build
system is to look for MousePaw Media's `libdeps/` and `pawlib/` repositories,
cloned parallel to this repository's main folder. Simply run `$ make ready` in
each of those repositories (optionally, you can run `$ make cpgf` in `libdeps/`)
before building this one. (This is our default for company development
environments.)

You can specify custom paths for these libraries by creating a `.config` file
in the root of this repository. Make a copy of `build.config.txt` and save it
with the ending `.config`. See that file for more information.

## Ready-To-Use Build

If you just want to build Ratscript to use in your own project, the fastest way is
to run `$ make ready`. This will build Ratscript and place it in a folder called
`ratscript/`, then point your compiler and linker to `ratscript/include` and
`ratscript/lib` respectively. Our other repositories point to this by default.

## Building HTML Docs

If you want the HTML documentation for Ratscript, run `$ make docs`. Then, grab the
`docs/build/html` folder, or just open `docs/build/html/index.html` in your
favorite web browser.

## Building Tester

If you want to test out Ratscript directly, run `make console`. Then, look for
the `rsconsole` executable in `console/bin/[Debug/Release]`. Alternatively, you
may use the symbolic link `rsconsole` or `rsconsole_debug` in the root of this
repository.

## IDE Support

Ratscript was written and built in CodeBlocks and VSCode. The CodeBlocks projects
(`*.cbp`) in this repository are pre-configured to build using the
Makefile/CMake build system. The configuration files in `.vscode/` are
pre-configured to provide the best IDE experience, and allow building and
debugging.

## Source Directories

- The `/docs` folder contains the Sphinx documentation for Ratscript.
- The `/language` folder contains the source code for the Ratscript
  static library.
- The `/console` folder contains the console application (REPL), which can also
  be used to test the Ratscript static library.