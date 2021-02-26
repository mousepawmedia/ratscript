# CMake Config (MousePaw Media Build System)
# Version: 3.1.0

# CHANGE: Name your project here
project("Ratscript Tester")

# Specify the verison being used.
cmake_minimum_required(VERSION 3.8)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# CHANGE: Specify output binary name
set(TARGET_NAME "ratscript-tester")

# SELECT: Project artifact type
#set(ARTIFACT_TYPE "library")
set(ARTIFACT_TYPE "executable")

# CHANGE: Find dynamic library dependencies.

# CHANGE: Include headers of dependencies.
set(INCLUDE_LIBS
    ${CMAKE_HOME_DIRECTORY}/../ratscript-source/include
    ${SIMPLEXPRESS_DIR}/include
    ${NIMBLY_DIR}/include
    ${GOLDILOCKS_DIR}/include
    ${IOSQUEAK_DIR}/include
    ${ONESTRING_DIR}/include
    ${ARCTICTERN_DIR}/include
    ${EVENTPP_DIR}/include
)

# CHANGE: Include files to compile.
set(FILES
    main.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
    ${CMAKE_HOME_DIRECTORY}/../ratscript-source/lib/${CMAKE_BUILD_TYPE}/libratscript.a
    ${SIMPLEXPRESS_DIR}/lib/libsimplexpress.a
    ${NIMBLY_DIR}/lib/libnimbly.a
    ${GOLDILOCKS_DIR}/lib/libgoldilocks.a
    ${IOSQUEAK_DIR}/lib/libiosqueak.a
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)
