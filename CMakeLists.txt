cmake_minimum_required(VERSION 3.7)
project(protection)

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

set(ANTLR4CPP_GRAMMAR_PATH "${PROJECT_SOURCE_DIR}/third_party/libprotection-grammars/Antlr")
set(ANTLR4CPP_JAR_LOCATION ${PROJECT_SOURCE_DIR}/third_party/antlr/antlr-4.7-complete.jar)

include(BuildAntlr4Cpp)
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
link_directories(${ANTLR4CPP_LIBS})
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS}")

add_definitions(-Wall -Werror -g -std=c++11 -fPIC -Wno-attributes)
remove_definitions(-Werror)

include_directories(include)
add_subdirectory(lib)

add_library(${PROJECT_NAME} SHARED
            lib/Range.cpp
            lib/Languages/RegexLanguageProvider.cpp
		        lib/Languages/RegexTokenDefinition.cpp
		        lib/Languages/Token.cpp
            lib/Languages/AntrlLanguageProvider.cpp)

target_link_libraries(${PROJECT_NAME} FilePath Url antlr4-runtime)
