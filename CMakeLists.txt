cmake_minimum_required(VERSION 3.7)

project(LIBPROTECTION C CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "32-bit build is not supported")
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(WARNING "Build type not set, falling back to Release mode.
 To specify build type use:
 -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
       "Choose the type of build, options are: Debug Release."
       FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#TODO
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(SUBMODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/submodules)

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  if (UNIX)
    add_definitions(-fno-sanitize-recover=all -fsanitize=address -fno-omit-frame-pointer)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGE} -fno-sanitize-recover=all -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGE} -fno-sanitize-recover=all -fsanitize=address -fno-omit-frame-pointer")
  endif()
endif()

set(PROTECTION_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
  set(WITH_STATIC_CRT OFF CACHE BOOL "Enable to statically link CRT.")
endif()

include(BuildAntlr4Cpp)

include_directories(${ANTLR4CPP_INCLUDE_DIRS})
link_directories(${ANTLR4CPP_LIB_DIRS})
message(STATUS "antlr4cpp libs: ${ANTLR4CPP_LIB_DIRS}
   antlr4cpp includes: ${ANTLR4CPP_INCLUDE_DIRS}")

# fmt header-only usage
set(FMT_ROOT ${SUBMODULES_DIR}/fmt)
add_subdirectory(${FMT_ROOT} EXCLUDE_FROM_ALL)

if(MSVC)
  add_definitions(/W3 /D_CRT_SECURE_NO_WARNINGS /WX- /DLIBPROTECTION_DLL /DANTLR4CPP_STATIC)
else()
  add_definitions(-Wall -Werror -fPIC -Wno-attributes)
endif()

include_directories(${PROTECTION_INCLUDE_DIR})
include_directories(${SUBMODULES_DIR})

add_subdirectory(lib)

option(PROTECTION_INCLUDE_TESTS "Generate build targets for the libprotection unit tests." ON)

if(PROTECTION_INCLUDE_TESTS)
  enable_testing()
  include(addTest)
  include(precompileCatch)
  precompileCatch(catch)
  add_subdirectory(unittests)
endif()

install(DIRECTORY ${PROTECTION_INCLUDE_DIR}/protection
      DESTINATION include
      COMPONENT protection-headers)
