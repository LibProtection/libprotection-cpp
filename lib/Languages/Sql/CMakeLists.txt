project(SqlLanguageProvider)

set(namespace sql)
set(lexer_grammar SQLLexer)
antlr4_add_grammar(${namespace} ${lexer_grammar})

add_library(${PROJECT_NAME} Sql.cpp)

add_dependencies(${PROJECT_NAME} ${namespace})
target_include_directories(${PROJECT_NAME} PUBLIC ${antlr4cpp_include_dirs_${namespace}})
target_link_libraries(${PROJECT_NAME} ${namespace})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if(WITH_STATIC_CRT)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MT$<$<CONFIG:Debug>:d>")
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
  endif()
endif()
