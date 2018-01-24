#
# precompileCatch(<variable>)
#
# Description:
# Precompile the Catch (framework for Unit-test), and set the given <variable>
# to the name of precompiled object lib target.
# NOTE: catch.hpp should be in 'include_directories'.
#
macro(precompileCatch target)
    if(NOT TARGET catch.precompiled)
        if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/catch.precompiled.cpp)
            file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/catch.precompiled.cpp
                "#define CATCH_CONFIG_MAIN\n#include <catch/single_include/catch.hpp>")
        endif()

        add_library(catch.precompiled OBJECT
            ${CMAKE_CURRENT_BINARY_DIR}/catch.precompiled.cpp)
    endif()

    set(${target} $<TARGET_OBJECTS:catch.precompiled>)
endmacro()
