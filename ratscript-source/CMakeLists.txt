# CMake Config (MousePaw Media Build System)
# Version: 3.1.0

# CHANGE: Name your project here
project("Ratscript")

# Specify the verison being used.
cmake_minimum_required(VERSION 3.8)

# Import user-specified library path configuration
message("Using ${CONFIG_FILENAME}.config")
include(${CMAKE_HOME_DIRECTORY}/../${CONFIG_FILENAME}.config)

# CHANGE: Specify output binary name
set(TARGET_NAME "ratscript")

# SELECT: Project artifact type
set(ARTIFACT_TYPE "library")
#set(ARTIFACT_TYPE "executable")

# CHANGE: Find dynamic library dependencies.

# CHANGE: Include headers of dependencies.
set(INCLUDE_LIBS
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
    include/ratscript/lexer.hpp
    include/ratscript/ratscript.hpp
    include/ratscript/tokens.hpp
    include/ratscript/token_types.hpp
    include/ratscript/tests/lexer_test.hpp
    src/lexer.cpp
    src/ratscript.cpp
    src/tokens.cpp
    src/token_types.cpp
    src/tests/lexer_test.cpp
#     include/ratscript/catalog.hpp
#     include/ratscript/conditional.hpp
#     include/ratscript/dev.hpp
#     include/ratscript/doublylinkedlist.hpp
#     include/ratscript/error.hpp
#     include/ratscript/evaluator_postfix.hpp
#     include/ratscript/math.hpp
#     include/ratscript/mem.hpp
#     include/ratscript/parser.hpp
#     include/ratscript/reserved.hpp
#     include/ratscript/scope.hpp
#     include/ratscript/sys.hpp
#     include/ratscript/utils.hpp
#     include/ratscript/var.hpp
#     src/catalog.cpp
#     src/conditional.cpp
#     src/dev.cpp
#     src/error.cpp
#     src/evaluator_postfix.cpp
#     src/math.cpp
#     src/mem.cpp
#     src/parser.cpp
#     src/scope.cpp
#     src/sys.cpp
#     src/utils.cpp
#     src/var.cpp
)

# CHANGE: Link against dependencies.
set(LINK_LIBS
    ${SIMPLEXPRESS_DIR}/lib/libsimplexpress.a
    ${NIMBLY_DIR}/lib/libnimbly.a
    ${GOLDILOCKS_DIR}/lib/libgoldilocks.a
    ${IOSQUEAK_DIR}/lib/libiosqueak.a
)

# Imports build script. (Change if necessary to point to build.cmake)
include(${CMAKE_HOME_DIRECTORY}/../build_system/build.cmake)
