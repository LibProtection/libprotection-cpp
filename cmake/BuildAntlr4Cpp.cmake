find_package(Java COMPONENTS Runtime REQUIRED)

set(ANTLR4CPP_GRAMMAR_PATH "${SUBMODULES_DIR}/libprotection-common/grammars")
set(ANTLR4CPP_JAR_LOCATION ${SUBMODULES_DIR}/libprotection-common/antlr/antlr-4.7-complete.jar)
set(ANTLR4_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/submodules/antlr4)
set(ANTLR4_CPP_ROOT_DIR ${ANTLR4_ROOT}/runtime/Cpp)

# default path for source files
if(NOT ANTLR4CPP_GENERATED_SRC_DIR)
  set(ANTLR4CPP_GENERATED_SRC_DIR ${CMAKE_BINARY_DIR}/antlr4cpp_generated_src)
endif()

list(APPEND ANTLR4CPP_INCLUDE_DIRS ${ANTLR4_CPP_ROOT_DIR}/runtime/src)
foreach(src_path  misc atn dfa tree support)
  list(APPEND ANTLR4CPP_INCLUDE_DIRS ${ANTLR4_CPP_ROOT_DIR}/runtime/src/${src_path})
endforeach(src_path)

file(GLOB_RECURSE ANTLR4CPP_SOURCES "${ANTLR4_CPP_ROOT_DIR}/runtime/src/*.cpp")

add_library(antlr4_shared SHARED ${ANTLR4CPP_SOURCES})
add_library(antlr4_static STATIC ${ANTLR4CPP_SOURCES})

install(TARGETS antlr4_shared DESTINATION lib)
install(TARGETS antlr4_static ARCHIVE DESTINATION lib)

set(static_lib_suffix "")
if(MSVC)
  set(static_lib_suffix "-static")
endif()

set_target_properties(antlr4_shared
                      PROPERTIES
                      OUTPUT_NAME antlr4-runtime)

set_target_properties(antlr4_static
                      PROPERTIES
                      OUTPUT_NAME "antlr4-runtime${static_lib_suffix}")

# macro for lexers generation
macro(antlr4cpp_process_grammar
      namespace
      grammar
      grammar_path)
  # Generated lexer
  string(REGEX MATCH "Lexer$" matches ${grammar})
  if (matches)
    set(lexer_cpp_file "${ANTLR4CPP_GENERATED_SRC_DIR}/${namespace}/${grammar}.cpp")
  else()
    set(lexer_cpp_file "${ANTLR4CPP_GENERATED_SRC_DIR}/${namespace}/${grammar}Lexer.cpp")
  endif()

  add_custom_command(
      OUTPUT
      ${lexer_cpp_file}
      PRE_BUILD
      COMMAND
      ${CMAKE_COMMAND} -E make_directory ${ANTLR4CPP_GENERATED_SRC_DIR}
      COMMAND
      ${Java_JAVA_EXECUTABLE}
      -jar
      ${ANTLR4CPP_JAR_LOCATION}
      -Werror
      -Dlanguage=Cpp
      -o
      ${ANTLR4CPP_GENERATED_SRC_DIR}/${namespace}
      -package
      ${namespace}
      ${grammar_path}/${grammar}.g4
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      DEPENDS ${grammar_path}/${grammar}.g4
  )
  add_custom_target(${namespace}_lexer ALL DEPENDS ${lexer_cpp_file})
  # Export generated file to build
  set(antlr4cpp_src_files_${namespace} ${lexer_cpp_file})
  if(NOT MSVC)
    set_source_files_properties(
        ${antlr4cpp_src_files_${namespace}}
        PROPERTIES
        COMPILE_FLAGS "-Wno-overloaded-virtual"
    )
  endif()
  # export generated include directory
  set(antlr4cpp_include_dirs_${namespace} ${ANTLR4CPP_GENERATED_SRC_DIR}/${namespace})
endmacro()

# macro for adding grammar into target dependencies
macro(antlr4_add_grammar
      namespace
      grammar)
  antlr4cpp_process_grammar(${namespace} ${grammar} ${ANTLR4CPP_GRAMMAR_PATH})
  add_library(${namespace} STATIC ${antlr4cpp_src_files_${namespace}})
  add_dependencies(${namespace} ${namespace}_lexer)
endmacro()
