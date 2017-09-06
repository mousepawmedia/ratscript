# Building Ratscript

Building Basics
----------------------
CMake is used to build Ratscript. For your convenience,
we've included Makefiles to automate all common build scenarios on Linux.
Type "make" in the root of this repository for more information.

At this time, the Makefiles are only designed for Linux. If you are building
on another system, you can interact with CMake directly.

Currently, Ratscript is only designed to be built by GCC (5.3 or later) or
Clang (3.4 or later).

## Building and Linking Dependencies

Ratscript relies on CPGF, PawLIB, and SIMPLEXpress. The build system's default
behavior is to look for MousePaw Media's `pawlib/`, `simplexpress/`, and
`libdeps/` repositories, cloned parallel to this repository's main folder.
Simply run `make ready` in each of those repositories before building this one.
(This is our default for company development environments.)

You can specify custom paths for these libraries by creating a ".config" file
in the root of this repository. Make a copy of "build.config.txt" and save it
with the ending ".config". See that file for more information.

## Ready-To-Use Build

If you just want to build Ratscript to use in your own project, the fastest way
is to run "make ready". This will build Ratscript and its documentation,
and place them all in a folder called "ratscript". You can use the compiled
interpreter there. Alternatively, to use the language as a static library,
simply copy that folder to a convenient location, and point your compiler and
linker to "ratscript/include" and "ratscript/lib" respectively.

## Building HTML Docs

If you want the HTML documentation, run "make docs". Then, grab the 'docs/build/html'
folder, or just open 'docs/build/html/index.html' in your favorite web browser.

For more documentation formats, see the Makefile in 'docs/'.

## Building Console

If you want to use Ratscript directly, run "make console". Then, look
for the rsconsole executable in console/bin/[Debug/Release].

## Code::Blocks

Ratscript was written and built in CodeBlocks. The projects (.cbp) in this
repository are pre-configured to build directly in the repository.

## Source Directories

- The '/docs' folder contains the Sphinx documentation for Ratscript.
- The '/language' folder contains the source code for the Ratscript language.
- The '/interactive' folder contains the interactive console application for
  Ratscript.
