include(CMakeParseArguments)

#
# addTest(test_name file_stem [OBJECT_LIBS libs...] [[LIBS] libs...])
#
# Parameters:
# test_name     -   test description string
# file_stem     -   test-file name stem string
# <<rest_args>> -   all rest, optional arguments are considered as
#                   library names or flags for target_link_libraries invokation.
#
# Description:
# Compailes the ${file_stem}.test.cpp to binary ${file_stem}.test
# and add this binary as test.
#
function(addTest test_name file_stem)
    cmake_parse_arguments(DEPS "" "" "LIBS;OBJECT_LIBS" ${ARGN})

    set(CPP_SOURCE_FILE_NAME
        "${file_stem}.test.cpp")

    get_filename_component(TEST_FILE_NAME ${file_stem} NAME)
    set(TEST_FILE_NAME "${TEST_FILE_NAME}.test")

    add_executable(${TEST_FILE_NAME}
        ${CPP_SOURCE_FILE_NAME}
        ${DEPS_OBJECT_LIBS})

    target_link_libraries(${TEST_FILE_NAME}
        ${DEPS_LIBS}
        ${DEPS_UNPARSED_ARGUMENTS})

    add_test(${test_name} ${TEST_FILE_NAME})
endfunction()
