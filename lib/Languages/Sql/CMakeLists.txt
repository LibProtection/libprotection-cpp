project(Sql)

set(namespace sql)
set(grammar_lexer SQLLexer)
antlr4cpp_process_grammar(${namespace} ${grammar_lexer} ${ANTLR4CPP_GRAMMAR_PATH})
add_library(${namespace} STATIC ${antlr4cpp_src_files_${namespace}})
add_dependencies(${namespace} ${namespace}_lexer)

add_library(${PROJECT_NAME} Sql.cpp)
add_dependencies(${PROJECT_NAME} ${namespace})
target_include_directories(${PROJECT_NAME} PUBLIC ${antlr4cpp_include_dirs_${namespace}})
target_link_libraries(${PROJECT_NAME} ${namespace})
