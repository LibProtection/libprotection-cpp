cmake_minimum_required(VERSION 3.7)
project(protection)

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

include_directories(include)
add_subdirectory(lib)

add_library(${PROJECT_NAME} SHARED
	lib/Range.cpp
        lib/Languages/RegexLanguageProvider.cpp
		lib/Languages/RegexTokenDefinition.cpp
		lib/Languages/Token.cpp
        )


target_link_libraries(${PROJECT_NAME} FilePath Url)
