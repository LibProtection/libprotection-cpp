cmake_minimum_required(VERSION 3.7)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" FORCE)
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  add_definitions(-fsanitize=address -fno-omit-frame-pointer)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGE} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGE} -fsanitize=address -fno-omit-frame-pointer")
endif()

set(ANTLR4CPP_GRAMMAR_PATH "${PROJECT_SOURCE_DIR}/submodules/libprotection-common/grammars")
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/submodules/libprotection-common/antlr/antlr-4.7-complete.jar)

include(BuildAntlr4Cpp)
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
link_directories(${ANTLR4CPP_LIBS})
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS}")

# fmt header-only usage
set(FMT_ROOT ${PROJECT_SOURCE_DIR}/submodules/fmt)
add_subdirectory(${FMT_ROOT} EXCLUDE_FROM_ALL)

add_definitions(-Wall -g -std=c++11 -fPIC -Wno-attributes)

include_directories(include)
include_directories(submodules)

add_subdirectory(lib)

enable_testing()
include(addTest)
include(precompileCatch)
precompileCatch(catch)
add_subdirectory(unittests)
