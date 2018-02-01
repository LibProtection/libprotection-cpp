project(JavaScript)

set(namespace javascript)
set(grammar_lexer JavaScriptCpp)
antlr4_add_grammar(${namespace} ${grammar_lexer})

add_library(${PROJECT_NAME} JavaScript.cpp)
add_dependencies(${PROJECT_NAME} ${namespace})
target_include_directories(${PROJECT_NAME} PUBLIC ${antlr4cpp_include_dirs_${namespace}})
target_link_libraries(${PROJECT_NAME} ${namespace} WebUtils)
